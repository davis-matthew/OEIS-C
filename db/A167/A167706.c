/*
 * A167706  The single or isolated numbers. The union of single (or isolated
 *          or non-twin) primes and single (or isolated or average of twin
 *          prime pairs) nonprimes.
 * https://oeis.org/A167706
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   A167706 – union of isolated primes and averages of twin prime
 *            pairs
 * Range:     u8 n <= 32, u16 n <= 5682, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* static caching ----------------------------------------------------------------*/

static idx_t         current_bound = 0; /* highest bound that has been built */
static seq_t        *seq_cache     = NULL; /* cached sequence values */
static size_t        seq_len       = 0;     /* number of cached values */

/* comparison function for qsort --------------------------------------------*/

static int cmp_seq(const void *p1, const void *p2)
{
    const seq_t *x = (const seq_t *)p1;
    const seq_t *y = (const seq_t *)p2;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

/* build all terms ≤ bound ------------------------------------------------*/

static void build_sequence(idx_t bound)
{
    /* free any previous cache */
    if (seq_cache) {
        free(seq_cache);
        seq_cache = NULL;
        seq_len   = 0;
    }

    /* sieve of Eratosthenes ---------------------------------------------------*/
    size_t size = (size_t)(bound + 1);
    uint8_t *is_prime = (uint8_t *)calloc(size, 1);
    if (!is_prime) return;  /* allocation failed */

    for (idx_t i = 2; i <= bound; ++i) is_prime[i] = 1;
    for (idx_t i = 2; (idx_t)i * i <= bound; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= bound; j += i) {
                is_prime[j] = 0;
            }
        }
    }

    /* store values --------------------------------------------------------*/
    size_t capacity = (size_t)(bound / 2 + 20);
    seq_t  *tmp     = (seq_t *)malloc(capacity * sizeof(seq_t));
    if (!tmp) { free(is_prime); return; }
    size_t len = 0;

    /* isolated primes (primes without twin) -----------------------------*/
    for (idx_t p = 2; p <= bound; ++p) {
        if (is_prime[p]) {
            bool has_twin = false;
            if (p > 2 && is_prime[p - 2]) has_twin = true;
            if (p + 2 <= bound && is_prime[p + 2]) has_twin = true;
            if (!has_twin) {
                if (len == capacity) {
                    capacity <<= 1;
                    seq_t *t2 = (seq_t *)realloc(tmp, capacity * sizeof(seq_t));
                    if (!t2) { free(tmp); free(is_prime); return; }
                    tmp = t2;
                }
                /* cast to seq_t: guaranteed to fit (p ≤ bound) */
                tmp[len++] = (seq_t)p;
            }
        }
    }

    /* averages of twin prime pairs ---------------------------------------*/
    for (idx_t p = 2; p + 2 <= bound; ++p) {
        if (is_prime[p] && is_prime[p + 2]) {
            seq_t val = (seq_t)(p + 1);            /* (p + (p+2)) / 2 = p+1 */
            if (len == capacity) {
                capacity <<= 1;
                seq_t *t2 = (seq_t *)realloc(tmp, capacity * sizeof(seq_t));
                if (!t2) { free(tmp); free(is_prime); return; }
                tmp = t2;
            }
            tmp[len++] = val;
        }
    }

    free(is_prime);

    /* sort the collected values ------------------------------------------*/
    qsort(tmp, len, sizeof(seq_t), cmp_seq);

    /* store result --------------------------------------------------------*/
    seq_cache     = tmp;
    seq_len       = len;
    current_bound = bound;
}

/* public function: a(n) ----------------------------------------------------*/

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;    /* undefined, return 0 */

    /* ensure cache contains at least n terms --------------------------------*/
    if (seq_len < n || current_bound == 0) {
        idx_t bound = (n > 30 ? (idx_t)(n * 20ULL) : 100ULL);
        while (true) {
            build_sequence(bound);
            if (seq_len >= n) break;
            bound <<= 1;           /* double if still insufficient */
        }
    }

    return seq_cache[n - 1];
}
