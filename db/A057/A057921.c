/*
 * A057921  Numbers k such that d(k+1) divides d(k), where d(k) is number of
 *          positive divisors of k.
 * https://oeis.org/A057921
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   not stated
 * Range:     u8 n <= 113, u16 n <= 9522, u32 n <= 9522, u64 n <= 9522,
 *            u128 n <= 9522, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A057921 – numbers k such that d(k+1) divides d(k) */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/*----- Static state -----------------------------------------------------*/
static idx_t current_k = 2;           /* next candidate k to test       */
static size_t limit = 0;              /* highest value for which dcnt is valid  */
static uint32_t *dcnt = NULL;         /* divisor counts for 0..limit     */
static seq_t *seq = NULL;             /* stored terms of the sequence    */
static size_t seq_size = 0;           /* number of terms stored          */
static size_t seq_alloc = 0;          /* allocated capacity of seq       */

/*----- Helper: rebuild divisor counts up to 'new_limit' ----------------*/
static void extend_sieve_to(size_t new_limit)
{
    if (new_limit <= limit)
        return;

    /* reallocate and recompute divisor counts from scratch */
    uint32_t *new_dcnt = (uint32_t *)realloc(dcnt, (new_limit + 1) * sizeof(uint32_t));
    if (!new_dcnt)
        abort();

    for (size_t i = 0; i <= new_limit; ++i)
        new_dcnt[i] = 0;

    for (size_t d = 1; d <= new_limit; ++d)
        for (size_t m = d; m <= new_limit; m += d)
            ++new_dcnt[m];

    dcnt = new_dcnt;
    limit = new_limit;
}

/*----- Ensure that at least 'needed' terms of the sequence are known ------*/
static void ensure_terms(idx_t needed)
{
    if (seq_size >= (size_t)needed)
        return;                           /* already have enough terms */

    /* Initialise divisor table if this is the first call */
    if (limit == 0)
        extend_sieve_to(200);               /* arbitrary small start */

    while (seq_size < (size_t)needed) {
        /* make sure divisor counts for k and k+1 exist */
        if ((size_t)current_k + 1 > limit) {
            size_t new_limit = limit * 2;
            if (new_limit < (size_t)current_k + 1)
                new_limit = (size_t)current_k + 1;
            extend_sieve_to(new_limit);
        }

        uint32_t d_k    = dcnt[(size_t)current_k];
        uint32_t d_next = dcnt[(size_t)current_k + 1];

        if (d_k % d_next == 0) {            /* k satisfies the condition */
            if (seq_size == seq_alloc) {    /* grow result array */
                seq_alloc = seq_alloc ? seq_alloc * 2 : 8;
                seq = (seq_t *)realloc(seq, seq_alloc * sizeof(seq_t));
                if (!seq)
                    abort();
            }
            seq[seq_size++] = (seq_t)current_k;
        }
        ++current_k;
    }
}

/*----- Public sequence function -------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;            /* undefined for n = 0, return safe value */

    ensure_terms(n);
    return seq[n - 1];
}
