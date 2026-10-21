/*
 * A066899  a(n) = card({k : phi(k) == n (mod k)}).
 * https://oeis.org/A066899
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = card({k : phi(k) == n (mod k)}), k > 1
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* size of the pre‑computed range for n (offset = 1) */
#define MAX_N 500000
/* maximum k value used for the sieve (ensure all φ(k) ≤ MAX_N) */
#define KMAX   3000000

static seq_t *phi_array = NULL;   /* Euler totient values */
static seq_t *count_arr = NULL;   /* pre‑computed a(n) values */
static bool initialized = false;

/* --------------------------------------------------------------------
   Initialise the totient table and the a(n) table.
   -------------------------------------------------------------------- */
static void init_sequence(void)
{
    size_t i, j;

    if (initialized)
        return;

    /* allocate totient array */
    phi_array = (seq_t *)malloc((KMAX + 1) * sizeof(seq_t));
    if (!phi_array) exit(1);

    for (i = 0; i <= (size_t)KMAX; ++i)
        phi_array[i] = (seq_t)i;

    /* Euler's sieve for totient */
    for (i = 2; i <= (size_t)KMAX; ++i) {
        if (phi_array[i] == (seq_t)i) {                /* i is prime */
            for (j = i; j <= (size_t)KMAX; j += i) {
                phi_array[j] = phi_array[j] / (seq_t)i * (seq_t)(i - 1);
            }
        }
    }

    /* allocate and zero the count array */
    count_arr = (seq_t *)malloc((MAX_N + 1) * sizeof(seq_t));
    if (!count_arr) exit(1);
    for (i = 0; i <= (size_t)MAX_N; ++i)
        count_arr[i] = 0;

    /* populate a(n) values from the totient data */
    for (j = 2; j <= (size_t)KMAX; ++j) {
        seq_t r = phi_array[j];
        if (r > (seq_t)MAX_N)          /* no contribution to stored range */
            continue;
        for (i = (size_t)r; i <= (size_t)MAX_N; i += j)
            ++count_arr[i];
    }

    initialized = true;
}

/* --------------------------------------------------------------------
   Main sequence function
   -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (!initialized) init_sequence();

    if (n <= (idx_t)MAX_N)              /* fast pre‑computed answer */
        return count_arr[n];

    /* fallback for larger n: search k up to KMAX */
    seq_t result = 0;
    for (idx_t k = 2; k <= (idx_t)KMAX; ++k) {
        if ( (seq_t)(n % k) == phi_array[k] )
            ++result;
    }
    return result;
}
