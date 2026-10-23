/*
 * A116905  Number of partitions of n-th 3-almost prime into 2 squares.
 * https://oeis.org/A116905
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = A000161(A014612(n))
 * Range:     u8 n <= 57, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of partitions of the n‑th 3‑almost prime into 2 squares. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ----------------------------------------------------------------------- */
/* Integer square root (floor) for uint64_t                                         */
static uint64_t isqrt_uint64(uint64_t v)
{
    uint64_t r = 0;
    uint64_t bit = 1ULL << 62;   /* largest power of four <= v */
    while (bit > v) bit >>= 2;
    while (bit != 0) {
        if (v >= r + bit) {
            v -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
        bit >>= 2;
    }
    return r;
}

/* ----------------------------------------------------------------------- */
/* Smallest prime factor table handling (rebuilds the whole table up to n).    */
static uint32_t *spf = NULL;          /* size spf_cap; spf[i] == 0 for i < 2          */
static size_t   spf_cap = 0;          /* current allocated size                      */

static void ensure_spf_upto(uint64_t n)
{
    if (spf_cap > n) return;                  /* already available */

    size_t new_cap = n + 1;
    uint32_t *new_spf = (uint32_t*)calloc(new_cap, sizeof(uint32_t));
    if (!new_spf) abort();                    /* allocation failure */

    for (uint64_t i = 2; i < new_cap; ++i) {
        if (new_spf[i] == 0) {
            new_spf[i] = (uint32_t)i;
            if (i * i < new_cap) {
                for (uint64_t j = i * i; j < new_cap; j += i)
                    if (new_spf[j] == 0)
                        new_spf[j] = (uint32_t)i;
            }
        }
    }
    free(spf);
    spf = new_spf;
    spf_cap = new_cap;
}

/* ----------------------------------------------------------------------- */
/* List of 3‑almost primes                                                     */
static uint64_t *three_primes = NULL;   /* dynamic array                                    */
static size_t   tp_len   = 0;           /* number of primes stored                         */
static size_t   tp_cap   = 0;           /* allocated capacity                              */
static uint64_t last_scanned = 1;       /* last integer examined for 3‑almostness         */

static void ensure_three_primes(size_t needed)
{
    if (tp_len >= needed) return;          /* already have enough                       */

    if (tp_cap < needed) {
        tp_cap = needed > 10 ? needed : 10;
        uint64_t *tmp = (uint64_t*)realloc(three_primes, tp_cap * sizeof(uint64_t));
        if (!tmp) abort();
        three_primes = tmp;
    }

    while (tp_len < needed) {
        ++last_scanned;
        if (last_scanned < 2) continue;    /* skip 0,1 */
        ensure_spf_upto(last_scanned);
        uint64_t x = last_scanned;
        int factor_cnt = 0;
        while (x > 1) {
            uint32_t p = spf[x];
            do { x /= p; ++factor_cnt; } while (x % p == 0);
        }
        if (factor_cnt == 3) {
            three_primes[tp_len++] = last_scanned;
        }
    }
}

/* ----------------------------------------------------------------------- */
/* Check whether an integer can be expressed as a sum of two squares.        */
static inline bool can_sum_two_squares(uint64_t m)
{
    if (m == 0) return true;
    ensure_spf_upto(m);
    while (m > 1) {
        uint32_t p = spf[m];
        int exp = 0;
        while (m % p == 0) { m /= p; ++exp; }
        if ((p & 3u) == 3u && (exp & 1) == 1) return false;
    }
    return true;
}

/* ----------------------------------------------------------------------- */
/* Count unordered pairs (x, y) with x^2 + y^2 = m, x,y >= 0.                */
static inline seq_t count_two_square_partitions(uint64_t m)
{
    uint64_t cnt = 0;
    uint64_t max_x = isqrt_uint64(m / 2);
    for (uint64_t x = 0; x <= max_x; ++x) {
        uint64_t xx = (uint64_t)(((__uint128_t)x) * x);
        if (xx > m) break;
        uint64_t rem = m - xx;
        uint64_t y = isqrt_uint64(rem);
        if ((__uint128_t)y * y == rem) ++cnt;
    }
    return (seq_t)cnt;
}

/* ----------------------------------------------------------------------- */
/* Main sequence function.                                                   */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* offset not used; return 0 for safety */
    size_t need = (size_t)n;
    ensure_three_primes(need);
    uint64_t target = three_primes[need - 1];
    if (!can_sum_two_squares(target)) return 0;
    return count_two_square_partitions(target);
}
