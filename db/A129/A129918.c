/*
 * A129918  Smallest prime of the form k*prime(n+1)+prime(n) =
 *          j*prime(n+2)+prime(n+1) for free integer multipliers k and j.
 * https://oeis.org/A129918
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 20, u32 n <= 902, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 53 terms through n = 992 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A129918:  smallest prime p for which
 *  p ≡ prime(n)   (mod prime(n+1))
 *  p ≡ prime(n+1) (mod prime(n+2))
 *
 *  For index n (1‑based) we solve the two congruences by the Chinese
 *  remainder theorem, obtain the least non‑negative solution x, and
 *  then search the arithmetic progression x + k·M (M = prime(n+1)·prime(n+2))
 *  for the first prime.  The search uses the arbitrary‑precision primality
 *  test is_prime_big.  The function works for any seq_t size permitted by
 *  the build and keeps all intermediate arithmetic in seq_t or 64‑bit
 *  unsigned integers only. */

/* --------------------------------------------------------------------- */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/is_prime_u64.h"

/* Return the k‑th prime (k ≥ 1).  On first call the prime list
   is built incrementally using trial division.  The list is
   stored in a static array which is expanded as necessary.   */
static uint32_t get_prime(idx_t k)
{
    static uint32_t *pr = NULL;     /* array of primes */
    static size_t sz   = 0;         /* number of primes stored */
    static size_t cap  = 0;         /* allocated capacity */
    static uint32_t cand = 3;       /* candidate after 2 */

    /* initialise the prime table on first use */
    if (sz == 0) {
        cap = 8;
        pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
        if (!pr) abort();
        sz = 0;
    }

    /* grow the array until we have at least k primes */
    while (sz < (size_t)k) {
        if (sz == 0) {                /* add the first prime */
            pr[sz++] = 2;
            continue;
        }

        /* find the next prime candidate */
        uint32_t p = cand;
        bool ok = true;
        for (size_t i = 0; i < sz; ++i) {
            uint32_t prime = pr[i];
            if ((uint64_t)prime * prime > (uint64_t)p) break;
            if (p % prime == 0) { ok = false; break; }
        }

        if (!ok) {                    /* not prime – keep searching */
            cand += 2;                /* only odd numbers remain */
            continue;
        }

        /* prime found – store it, grow table if needed */
        if (sz == cap) {
            cap <<= 1;
            pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
            if (!pr) abort();
        }
        pr[sz++] = p;
        cand = p + 2;
    }

    return pr[(size_t)k - 1];
}

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

#if defined(__GNUC__) || defined(__clang__)
#  define INT128 signed __int128
#  define UINT128 unsigned __int128
#else
#  error "Compiler does not support 128‑bit integer type."
#endif

/* --------------------------------------------------------------------- */
/*  Extended Euclidean algorithm to compute the modular inverse of a
 *  modulo m.  Both a and m fit in 64 bits and are coprime (they are
 *  consecutive primes, hence coprime).  The result is in [0, m‑1].  */
static inline uint64_t modinv_u64(uint64_t a, uint64_t m)
{
    INT128 t    = 0;
    INT128 newt = 1;
    INT128 r    = (INT128)m;
    INT128 newr = (INT128)a;

    while (newr != 0) {
        uint64_t q     = (uint64_t)(r / newr);
        INT128 left    = t    - (INT128)q * newt;
        t              = newt;
        newt           = left;
        left            = r    - (INT128)q * newr;
        r              = newr;
        newr           = left;
    }
    if (r != 1) return 0;           /* inverse does not exist – should
                                        not happen for prime moduli */
    if (t < 0) t += m;
    return (uint64_t)t;
}

/* --------------------------------------------------------------------- */
/*  Compute a(n) for a single 1‑based index n.  The function is safe
 *  for any n >= 1.  If n < 1 the function returns 0 (undefined). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined; safe fallback */

    /* 1. Get the three consecutive primes. */
    uint32_t p  = get_prime(n);     /* prime(n)   */
    uint32_t p1 = get_prime(n + 1); /* prime(n+1) */
    uint32_t p2 = get_prime(n + 2); /* prime(n+2) */

    /* 2. Compute modular inverse of p1 modulo p2 and the CRT offset. */
    uint64_t inv  = modinv_u64(p1, p2);   /* inverse of p1 modulo p2 */

    /* Difference r2 - r1 mod p2 (positive). */
    int64_t diff = (int64_t)p1 - (int64_t)p;   /* p1 - p */
    diff %= (int64_t)p2;
    if (diff < 0) diff += p2;

    uint64_t t = (uint64_t)((UINT128)diff * inv % p2);  /* CRT coefficient */

    /* Least non‑negative solution of the two congruences. */
    seq_t x = (seq_t)p + (seq_t)p1 * (seq_t)t;
    seq_t M = (seq_t)p1 * (seq_t)p2;        /* modulus of the progression */
    x %= M;
    if (x <= (seq_t)1) x += M;              /* avoid 0 or 1 */

    /* 3. Search the arithmetic progression x + k·M for the first prime. */
    seq_t candidate = x;
    while (!is_prime_big(candidate))
        candidate += M;

    return candidate;
}
