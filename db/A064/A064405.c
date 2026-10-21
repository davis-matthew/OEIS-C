/*
 * A064405  Number of even entries (A048967) minus the number of odd entries
 *          (A001316) in row n of Pascal's triangle (A007318).
 * https://oeis.org/A064405
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign, look
 * Formula:   not stated
 * Range:     i8 n <= 135, i16 n <= 8191, i32 n <= 8191, i64 n <= 8191,
 *            i128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A064405: a(n) = Σ_{k=0..n} (-1)^{C(n,k)}
   Recurrence: a(0) = -1, a(1) = -2,
               a(2m)   = a(m) + m,
               a(2m+1) = 2 * a(m). */
#include <stdlib.h>

seq_t a(idx_t n) {
    /* static memoisation table */
    static seq_t* memo = NULL;        /* computed values */
    static unsigned char* done = NULL;/* flags whether memo[i] is valid */
    static size_t memo_size = 0;      /* current table capacity */

    /* grow the memoisation table if needed */
    if ((size_t)n >= memo_size) {
        size_t new_size = n + 1;
        seq_t* new_mem = (seq_t*)realloc(memo, new_size * sizeof(seq_t));
        unsigned char* new_done = (unsigned char*)realloc(done, new_size * sizeof(unsigned char));
        if (!new_mem || !new_done) {
            /* allocation failed – abort (caller should handle this ) */
            exit(EXIT_FAILURE);
        }
        memo = new_mem;
        done = new_done;
        /* initialise new entries as not computed */
        for (size_t i = memo_size; i < new_size; ++i)
            done[i] = 0;
        memo_size = new_size;
    }

    if (done[n])          /* already computed */
        return memo[n];

    seq_t result;
    if (n == 0) {
        result = -(seq_t)1;
    } else if (n == 1) {
        result = -(seq_t)2;
    } else if ((n & 1ULL) == 0) {           /* even n = 2*m */
        idx_t m = n >> 1;
        result = a(m) + (seq_t)m;
    } else {                                 /* odd n = 2*m+1 */
        idx_t m = (n - 1) >> 1;
        result = (seq_t)2 * a(m);
    }

    memo[n] = result;
    done[n] = 1;
    return result;
}
