/*
 * A128442  Array T by antidiagonals: T(n,k)=Floor(k*2^(n/2)).
 * https://oeis.org/A128442
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = floor(k*2^(i/2)) where (i,k) are the antidiagonal
 *            coordinates of the n-th entry (1‑based)
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* 160‑bit unsigned integer: low 128 bits + high 32 bits */
typedef struct {
    __uint128_t low;
    uint32_t   high;
} Uint160;

/* Multiply an __uint128_t by itself, result in a Uint160 */
static inline Uint160 mul_square_uint128(__uint128_t x)
{
    uint64_t lo   = (uint64_t)x;
    uint64_t hi   = (uint64_t)(x >> 64);          /* hi fits in 16 bits */

    __uint128_t ll = (__uint128_t)lo * lo;        /* low part  */
    __uint128_t lh = (__uint128_t)lo * hi;        /* cross terms  */
    __uint128_t hh = (__uint128_t)hi * hi;        /* high part    */

    /* middle term is 2*lh */
    __uint128_t mid = lh + lh;

    /* carry from low part */
    uint64_t carry_low = (uint64_t)(ll >> 64);

    /* add carry to middle term (also 128‑bit) */
    __uint128_t mid_total = mid + carry_low;

    Uint160 r;
    r.low   = ll;
    r.high  = (uint32_t)(mid_total >> 64) + (uint32_t)hh;  /* fits in 32 bits */
    return r;
}

/* Double a Uint160 value */
static inline Uint160 double_uint160(Uint160 u)
{
    Uint160 r;
    r.low   = u.low << 1;
    uint64_t carry = (u.low >> 127) & 1;
    r.high  = u.high * 2 + (uint32_t)carry;
    return r;
}

/* Compare two Uint160 values: return true if a <= b */
static inline bool uint160_leq(const Uint160 *a, const Uint160 *b)
{
    if (a->high < b->high) return true;
    if (a->high > b->high) return false;
    return a->low <= b->low;
}

/* Compute floor(A * sqrt(2)) for 0 <= A <= 2^78 (fits in 128 bits).
   The result fits in 128 bits. */
static inline seq_t floor_sqrt2(__uint128_t A)
{
    if (A == 0) return (seq_t)0;

    __uint128_t low   = 0;
    __uint128_t high  = A << 1;          /* sqrt(2) < 1.5 -> 2*A is a safe upper bound */
    Uint160 target = double_uint160(mul_square_uint128(A));

    __uint128_t best = 0;
    while (low <= high) {
        __uint128_t mid = low + ((high - low) >> 1);
        Uint160 midSq = mul_square_uint128(mid);
        if (uint160_leq(&midSq, &target)) {
            best = mid;
            low  = mid + 1;
        } else {
            if (mid == 0) break;      /* avoid underflow */
            high = mid - 1;
        }
    }
    return (seq_t)best;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Find diagonal sum s such that n lies on or before that diagonal */
    idx_t s = 1;
    while ((((s - 1) * s) / 2) < n) ++s;

    /* Number of terms before diagonal s */
    idx_t prev = (((s - 2) * (s - 1)) / 2);

    /* Position within diagonal s (0‑based) */
    idx_t pos = n - prev - 1;

    /* Coordinates (i,j) of the entry */
    idx_t i = pos + 1;
    idx_t j = s - pos - 1;

    if ((i & 1) == 0) {                  /* even row: integer power of 2 */
        idx_t m = i >> 1;
        __uint128_t val128 = ((__uint128_t)j) << m;
        return (seq_t)val128;
    } else {                              /* odd row: multiply by sqrt(2) */
        __uint128_t A = ((__uint128_t)j) << ((i - 1) >> 1);
        return floor_sqrt2(A);
    }
}
