/*
 * A201908  Irregular triangle of 2^k mod (2n-1).
 * https://oeis.org/A201908
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   not stated
 * Range:     u8 n <= 4439, u16 n <= 4439, u32 n <= 4439, u64 n <= 4439,
 *            u128 n <= 4439, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A201908: Irregular triangle of 2^k mod (2n-1)
    The sequence a(n) is obtained by concatenating, for n = 1,2,3,…,
    the cycle of powers of two modulo (2n-1) (with 0 for n = 1).
    Each cycle starts and ends with 1 (except for the first element 0).
    The function below builds the sequence lazily and memoises
    previously computed values so that any call with 1 ≤ n is answered
    in O(n) time and O(n) memory (n ≤ 10000 satisfies the limits).  */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Memoised cache: data[0] = a(1), data[1] = a(2), …            */
    static seq_t *data = NULL;
    static size_t data_size = 0;      /* number of elements stored     */
    static size_t data_cap  = 0;      /* current capacity              */
    static idx_t  next_row    = 2;    /* next n for which a cycle will be generated */

    /* Initialise cache with first term a(1) = 0   */
    if (data == NULL) {
        data_cap = 128;
        data     = (seq_t *)malloc(data_cap * sizeof(seq_t));
        if (!data) return 0;  /* out of memory – return 0 as a safe default */
        data[0] = 0;
        data_size = 1;
    }

    /* Append cycles until we have at least n elements          */
    while (data_size < (size_t)n) {
        /* Compute the next cycle corresponding to odd modulus M = 2*next_row-1   */
        if (next_row == 1) {    /* Special case for the very first row (n=1)          */
            if (data_size == data_cap) {
                data_cap *= 2;
                data = (seq_t *)realloc(data, data_cap * sizeof(seq_t));
            }
            data[data_size++] = 0;  /* a(1) = 0 already stored, just for completeness */
            next_row++;
            continue;
        }

        seq_t M = (seq_t)(2*next_row - 1);   /* odd modulus, M ≥ 3                 */

        /* Generate the cycle of powers of two modulo M      */
        seq_t val = 1;
        if (data_size == data_cap) {
            data_cap *= 2;
            data = (seq_t *)realloc(data, data_cap * sizeof(seq_t));
        }
        data[data_size++] = val;              /* first element of the cycle = 1 */

        while (1) {
            val = (val * 2) % M;              /* next power of two mod M          */
            if (val == 1) break;              /* cycle closed                      */
            if (data_size == data_cap) {
                data_cap *= 2;
                data = (seq_t *)realloc(data, data_cap * sizeof(seq_t));
            }
            data[data_size++] = val;
        }
        next_row++;
    }

    return data[n-1];
}
