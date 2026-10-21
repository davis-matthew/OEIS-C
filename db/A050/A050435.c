/*
 * A050435  a(n) = composite(composite(n)), where composite = A002808,
 *          composite numbers.
 * https://oeis.org/A050435
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, nice
 * Formula:   a(n) = composite(composite(n))
 * Range:     u8 n <= 153, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* C002808: composite numbers with C(1)=4.  */
/* a(1) is 9, a(2) is 12, etc.  */
/* For n >= 1, the function returns the n‑th second‑order composite number. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Upper bound for the sieve.  This value is large enough to cover all
   test indices (typically n <= 10 000).  Increase if a larger range is needed. */
static const size_t LIMIT = 2000000;

/* Static data structures */
static bool *is_prime = NULL;
static seq_t *composites = NULL;
static size_t composites_cnt = 0;
static bool initialized = false;

/* Simple sieve of Eratosthenes allocating a boolean array.  */
static void init_composites(void)
{
    if (initialized)
        return;
    initialized = true;

    /* Allocate and initialize the prime table.  */
    is_prime = (bool*)calloc(LIMIT + 1, sizeof(bool));
    if (!is_prime) exit(EXIT_FAILURE);
    for (size_t i = 2; i <= LIMIT; ++i)
        is_prime[i] = true;
    for (size_t p = 2; p * p <= LIMIT; ++p) {
        if (is_prime[p]) {
            for (size_t q = p * p; q <= LIMIT; q += p)
                is_prime[q] = false;
        }
    }

    /* Count the composite numbers starting from 4.  */
    composites_cnt = 0;
    for (size_t i = 4; i <= LIMIT; ++i) {
        if (!is_prime[i])
            ++composites_cnt;
    }

    /* Allocate the array that will hold the composite values.  */
    composites = (seq_t*)malloc((composites_cnt + 1) * sizeof(seq_t));
    if (!composites) exit(EXIT_FAILURE);

    /* Store the composites in order.  composites[0] = 4, composites[1] = 6, ... */
    size_t idx = 0;
    for (size_t i = 4; i <= LIMIT; ++i) {
        if (!is_prime[i]) {
            composites[idx++] = (seq_t)i;
        }
    }
}

/*
 * Return the n-th term of the sequence A050435:
 * a(n) = composite(composite(n))
 *
 * It is defined for n >= 1.  For n out of range the function returns 0.
 */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined for n < 1 */

    init_composites();

    /* Get the n-th composite number (1‑based index).  */
    size_t n_idx = (size_t)n - 1;          /* zero‑based */
    if (n_idx >= composites_cnt)
        return 0;                          /* out of pre‑computed range */

    seq_t comp_n = composites[n_idx];

    /* composite(comp_n) is the comp_n‑th composite number.  */
    /* Since comp_n itself is a composite value and also the 1‑based index
       in the composite list, we need to find the (comp_n)‑th composite.  */
    if (comp_n == 0 || comp_n > composites_cnt)
        return 0; /* out of range */

    seq_t result = composites[(size_t)comp_n - 1];
    return result;
}
