/*
 * A073432  Numbers k such that prime(k) + composite(k) is even.
 * https://oeis.org/A073432
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n‑th integer k that satisfies *
 *            prime(k)+composite(k) is even. * For k = 1 this always holds. *
 *            For k > 1 it holds iff the k‑th composite number is odd
 * Range:     u8 n <= 96, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* ------------------------------------------------------------------ */
/*  Static cache of the composite numbers.  The limit is ample for
 *  n up to 10 000 (sequence term ≈ 2n).  The sieve is built once.     */
/* ------------------------------------------------------------------ */
static const idx_t SIEVE_LIMIT = 500000;   /* adjusts if larger n is needed */
static idx_t *composites = NULL;
static size_t composites_count = 0;
static bool cache_prepared = false;

static void prepare_composites(void)
{
    if (cache_prepared) return;
    /* ---------- Sieve of Eratosthenes ---------- */
    unsigned char *is_prime = (unsigned char *)malloc(SIEVE_LIMIT + 1);
    if (!is_prime) return;                     /* failed allocation */
    memset(is_prime, 1, SIEVE_LIMIT + 1);
    is_prime[0] = is_prime[1] = 0;

    for (idx_t p = 2; p * p <= SIEVE_LIMIT; ++p) {
        if (is_prime[p]) {
            for (idx_t x = p * p; x <= SIEVE_LIMIT; x += p)
                is_prime[x] = 0;
        }
    }
    /* ---------- Collect composites ---------- */
    composites = (idx_t *)malloc((SIEVE_LIMIT - 3) * sizeof(idx_t));
    if (!composites) { free(is_prime); return; }
    composites_count = 0;
    for (idx_t v = 4; v <= SIEVE_LIMIT; ++v) { /* 4 is the first composite */
        if (!is_prime[v])
            composites[composites_count++] = v;
    }
    free(is_prime);
    cache_prepared = true;
}

/* ------------------------------------------------------------------ */
/*  Main sequence function                                            */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1, behaviour here unimportant */

    prepare_composites();

    idx_t good = 0;                 /* how many qualifying k's found */
    for (idx_t k = 1; k <= composites_count; ++k) {
        if (k == 1) {               /* prime(1)+composite(1) = 2 + 4 is even */
            ++good;
            if (good == n) return (seq_t)k;
        } else {
            /* for k > 1, we need the k‑th composite to be odd */
            if (composites[k - 1] & 1) {
                ++good;
                if (good == n) return (seq_t)k;
            }
        }
    }
    /* If we reach here, the precomputed span was too small.
     * For the intended problem constraints this should never happen. */
    return 0;
}
