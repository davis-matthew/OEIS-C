/*
 * A177458  The number of positive integers m for which the exponents of
 *          prime(n) and prime(n+1) in the prime power factorization of m! are
 *          both powers of 2.
 * https://oeis.org/A177458
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: partial, pass, 52 terms through n = 54 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A177458 – Number of m such that the exponents of prime(n)
    and prime(n+1) in the factorisation of m! are both powers of 2.
    a(n) = |{ m≥1 : v_{prime(n)}(m!) and v_{prime(n+1)}(m!) are
             powers of 2 }|   (offset 3)                               */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Legendre's formula: exponent of prime p in m! */
static inline idx_t exponent_in_fact(idx_t m, idx_t p)
{
    idx_t e = 0;
    while (m) {
        m /= p;
        e += m;
    }
    return e;
}

/* ------------------------------------------------------------------
   Return the minimal m (m ≥ 0) such that exponent_in_fact(m,p) ≥ t.
   If t = 0 return 0.
   If no such m exists within idx_t limits (overflow), return idx_t_max.
   ------------------------------------------------------------------ */
static idx_t find_min_m_for_exp(idx_t p, idx_t t)
{
    if (t == 0) return 0;
    idx_t lo = 0;
    idx_t hi = 1;
    /* Exponent grows monotonically; increase hi until it suffices. */
    while (exponent_in_fact(hi, p) < t) {
        if (hi > ((idx_t)-1) / 2) return ((idx_t)-1);  /* overflow */
        hi <<= 1;
    }
    /* Binary search to find the first m with exponent ≥ t. */
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if (exponent_in_fact(mid, p) >= t)
            hi = mid;
        else
            lo = mid + 1;
    }
    return lo;
}

/* ------------------------------------------------------------ */
/* Simple sieve to obtain primes up to a fixed limit. The limit
   is chosen large enough for n ≤ 10000 (the 10001‑st prime is 104743). */
#define SIEVE_LIMIT 200000   /* > 104743 */

static bool sieve_done = false;
static uint32_t primes[SIEVE_LIMIT];
static size_t primes_cnt = 0;

static void init_sieve(void)
{
    if (sieve_done) return;
    bool *is_composite = (bool *)calloc(SIEVE_LIMIT, sizeof(bool));
    if (!is_composite) exit(1);   /* memory exhausted – abort */
    size_t i, j;
    for (i = 2; i < SIEVE_LIMIT; ++i) {
        if (!is_composite[i]) {
            primes[primes_cnt++] = (uint32_t)i;
            if (i <= SIEVE_LIMIT / i) {
                for (j = i * i; j < SIEVE_LIMIT; j += i)
                    is_composite[j] = true;
            }
        }
    }
    free(is_composite);
    sieve_done = true;
}

/* ------------------------------------------------------------ */

/* Structure to hold a contiguous interval [L,R] of m producing a
   particular exponent.  Intervals are disjoint for a fixed prime.    */
typedef struct { idx_t L, R; } Interval;

/* ------------------------------------------------------------------
   Build all intervals [L,R] for which exponent of p in m! is a power
   of 2.  The array `arr` must have room for at least 64 intervals.
   The number of valid intervals is stored in *len.                 */
static void build_intervals(idx_t p, Interval *arr, size_t *len)
{
    *len = 0;
    for (int k = 0; k < 62; ++k) {          /* 2^k up to 2^61 */
        idx_t t = (idx_t)1 << k;           /* target exponent (power of 2) */
        idx_t low = find_min_m_for_exp(p, t);
        if (low == ((idx_t)-1)) break;     /* overflow – terminate */
        if (exponent_in_fact(low, p) != t) /* safety: skip if power not hit */
            continue;
        idx_t high = find_min_m_for_exp(p, t + 1);
        if (high == ((idx_t)-1))           /* no upper bound – extend to max */
            high = ((idx_t)-1);
        else
            high -= 1;
        if (high >= low) {
            arr[*len].L = low;
            arr[*len].R = high;
            ++(*len);
            if (*len >= 63) break;         /* safety bound */
        }
    }
}

/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 3) return (seq_t)0;            /* offset 3 – below sequence not defined */

    init_sieve();                           /* primes ready */

    /* Prime indices in OEIS: prime(1)=2 corresponds to primes[0] */
    idx_t p = (idx_t)primes[(size_t)(n - 1)];     /* prime(n)   */
    idx_t q = (idx_t)primes[(size_t)n];          /* prime(n+1) */

    Interval ints_p[64], ints_q[64];
    size_t len_p, len_q;

    build_intervals(p, ints_p, &len_p);
    build_intervals(q, ints_q, &len_q);

    seq_t count = 0;
    size_t i = 0, j = 0;
    while (i < len_p && j < len_q) {
        idx_t L = (ints_p[i].L > ints_q[j].L) ? ints_p[i].L : ints_q[j].L;
        idx_t R = (ints_p[i].R < ints_q[j].R) ? ints_p[i].R : ints_q[j].R;
        if (L <= R) count += (seq_t)(R - L + 1);
        if (ints_p[i].R < ints_q[j].R)
            ++i;
        else
            ++j;
    }
    return count;
}
