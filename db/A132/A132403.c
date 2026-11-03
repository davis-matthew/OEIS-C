/*
 * A132403  Triangle read by rows: T(n,k) = nextprime( T(n-1,k) + T(n-1,k-1)
 *          ), where nextprime = A151800.
 * https://oeis.org/A132403
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) is the nth element (row‑major) of the triangle * defined by
 *            * T(0,0) = 1, * T(n,k) = nextprime( T(n-1,k) + T(n-1,k-1) ) for
 *            n>0, * with zeros padded beyond the borders (k<0 or k>n)
 * Range:     u8 n <= 38, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Validated: partial, pass, 66 terms through n = 65 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation stores the triangle in a contiguous cache
 * and appends new rows as needed.  next_prime_seq_t() finds the
 * least prime strictly greater than its argument using the build‑
 * provided is_prime_big() routine. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

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

static seq_t *cache     = NULL;   /* triangle entries, row‑major */
static size_t cache_len = 0;      /* number of elements stored   */
static size_t cache_cap = 0;      /* allocated capacity          */
static idx_t  last_row   = 0;     /* highest fully computed row  */

/* Next prime strictly greater than x (generic for seq_t). */
static inline seq_t next_prime_seq_t(seq_t x)
{
    seq_t cand = x + 1;
    while (!is_prime_big(cand))
        cand++;
    return cand;
}

/* a(n) – the OEIS sequence value at index n.  This function
 * is memoised; repeated calls do not recompute earlier rows.
 */
seq_t a(idx_t n)
{
    /* Initialise on first use. */
    if (cache == NULL) {
        cache_cap = 2;
        cache     = mem_new(seq_t, cache_cap);
        if (!cache)
            return 0;                 /* OOM – unlikely in test harness */
        cache[0]   = 1;                /* T(0,0) */
        cache_len  = 1;
        last_row   = 0;
    }

    /* Favour table reuse – compute rows until we cover index n. */
    while (cache_len <= n) {
        idx_t r = last_row + 1;                    /* new row to compute   */
        /* start index of previous row */
        idx_t prev_start = (r == 0) ? 0 :
                           ( (r-1) * r ) / 2;       /* tri(r-1) with idx_t arithmetic          */
        size_t needed = cache_len + (size_t)(r + 1);

        if (needed > cache_cap) {
            size_t new_cap = cache_cap * 2;
            while (new_cap < needed)
                new_cap <<= 1;
            seq_t *tmp = mem_resize(seq_t, cache, new_cap);
            if (!tmp)
                return 0;                     /* OOM – propagate failure                     */
            cache     = tmp;
            cache_cap = new_cap;
        }

        for (idx_t k = 0; k <= r; ++k) {
            seq_t left  = (k > 0)  ? cache[prev_start + k - 1] : 0;
            seq_t right = (k < r) ? cache[prev_start + k]     : 0;
            seq_t sum   = left + right;
            cache[cache_len++] = next_prime_seq_t(sum);
        }
        last_row = r;
    }

    return cache[n];
}
