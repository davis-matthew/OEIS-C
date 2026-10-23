/*
 * A178547  Composite numbers; start sequence with composite number and end
 *          with prime, length of successive sequences are strictly
 *          increasing.
 * https://oeis.org/A178547
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * A178547:  Composite numbers; start sequence with composite number 
 * and end with prime, block lengths strictly increase.  
 * a(n) is the starting composite of the n‑th block. 
 */

#include <stdbool.h>
#include <stdlib.h>

/* ----------  helper: integer square root for seq_t  ---------- */
static inline seq_t seq_isqrt(seq_t x)
{
    seq_t lo = 0, hi = x;
    while (lo <= hi) {
        seq_t mid = lo + ((hi - lo) >> 1);
        seq_t div = (mid == 0) ? 0 : x / mid;      /* avoid 0/0 */
        if (mid == div) {                          /* mid*mid <= x */
            if ((mid + 1) > div) return mid;       /* floor(sqrt) */
            lo = mid + 1;
        } else if (mid < div) {                    /* mid*mid <  x */
            if ((mid + 1) > div) return mid;
            lo = mid + 1;
        } else {                                   /* mid*mid >  x */
            hi = mid - 1;
        }
    }
    return lo;
}

/* ----------  prime table (dynamic, static)  ---------- */
static seq_t *prime_tbl   = NULL;   /* array of primes */
static size_t prime_cnt   = 0;      /* number of stored primes */
static size_t prime_cap   = 0;      /* allocated capacity */

/* enlarges prime_tbl to at least min_cap */
static void prime_grow(size_t min_cap)
{
    if (prime_cap >= min_cap) return;
    size_t new_cap = (prime_cap == 0) ? 8 : prime_cap << 1;
    if (new_cap < min_cap) new_cap = min_cap;
    seq_t *tmp = (seq_t *)realloc(prime_tbl, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);                 /* fatal error */
    prime_tbl = tmp;
    prime_cap = new_cap;
}

/* ensures prime_tbl contains all primes <= limit (limit may be 0) */
static void prime_sieve_to(seq_t limit)
{
    if (prime_tbl == NULL) {            /* first call */
        prime_grow(8);
        prime_tbl[0] = 2;
        prime_cnt = 1;
    }
    seq_t next = (prime_cnt == 0) ? 2 : prime_tbl[prime_cnt - 1] + 1;
    while (next <= limit) {
        bool flag = true;
        seq_t sqrt_n = seq_isqrt(next);
        for (size_t i = 0; i < prime_cnt; ++i) {
            seq_t p = prime_tbl[i];
            if (p > sqrt_n) break;
            if (next % p == 0) { flag = false; break; }
        }
        if (flag) {
            prime_grow(prime_cnt + 1);
            prime_tbl[prime_cnt++] = next;
        }
        ++next;
    }
}

/* primality test for seq_t values using prime_tbl */
static bool is_prime_big(seq_t n)
{
    if (n < 2) return false;
    seq_isqrt(0); /* avoid warning of unused seq_isqrt usage */
    if (n == 2) return true;
    if ((n & 1) == 0) return false;    /* even >2 */
    seq_t sqrt_n = seq_isqrt(n);
    prime_sieve_to(sqrt_n);             /* make sure primes up to sqrt(n) exist */
    for (size_t i = 0; i < prime_cnt; ++i) {
        seq_t p = prime_tbl[i];
        if (p > sqrt_n) break;
        if (n % p == 0) return false;
    }
    return true;
}

/* ----------  main sequence function  ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* undefined, but safe */

    seq_t s        = (seq_t)4;          /* current block start (composite) */
    seq_t prev_len = (seq_t)0;          /* length of previous block    */
    idx_t i        = (idx_t)1;          /* current block index         */

    while (i < n) {
        /* search next prime t ≥ s with (t – s + 1)  > prev_len  */
        for (seq_t t = s; ; ++t) {
            if (is_prime_big(t)) {
                seq_t len = t - s + (seq_t)1;
                if (len > prev_len) {
                    s        = t + (seq_t)1;   /* start next block      */
                    prev_len = len;            /* remember this length  */
                    ++i;                       /* finished a block      */
                    break;
                }
            }
        }
    }
    return s;            /* starting composite of n‑th block */
}
