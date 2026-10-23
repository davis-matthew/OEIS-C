/*
 * A171086  A "run" means a maximal string of numbers that includes exactly
 *          one odd number; this sequence has the property that the sequence
 *          of run lengths is the sequence itself and all natural numbers
 *          appear exactly once.
 * https://oeis.org/A171086
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 143, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes OEIS sequence A171086
 *  A(1) = 1.  The sequence is built as follows:
 *  * The sequence is split into runs, each run containing exactly one odd number.
 *  * The length of the k‑th run is a(k).
 *  * The odd number of the k‑th run is the smallest unused odd number.
 *  * After the odd number, the run is padded with the smallest unused even numbers.
 *
 *  This construction yields the required permutation with run‑lengths equal to the
 *  sequence itself.  The algorithm runs in linear time.
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                      /* undefined for n < 1 */

    /* 1‑based array to hold computed values up to n  */
    seq_t *a = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!a)
        return 0;                      /* allocation failure */

    idx_t cur_index = 1;                /* current position in the sequence */
    idx_t run_index = 1;                /* index of the run we are building */
    seq_t next_odd = 1;                 /* next odd number to use */
    idx_t even_counter = 1;             /* used to generate consecutive evens (value = 2 * counter) */

    while (cur_index <= n) {
        /* place the odd number of this run */
        if (cur_index <= n)
            a[cur_index] = next_odd;

        /* run length is a(run_index); it is already known because run_index ≤ cur_index */
        seq_t run_len_seq = a[run_index];
        idx_t run_len = (idx_t)run_len_seq;          /* run length as an idx_t */

        /* fill remaining even numbers of this run */
        idx_t evens_needed = (run_len > 0) ? (run_len - 1) : 0;
        idx_t pos = cur_index + 1;
        while (evens_needed > 0 && pos <= n) {
            a[pos] = (seq_t)(2 * even_counter);
            even_counter++;
            evens_needed--;
            pos++;
        }

        /* advance to the next run */
        next_odd += 2;
        cur_index += run_len;
        run_index++;
    }

    seq_t result = a[n];
    free(a);
    return result;
}
