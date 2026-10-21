/*
 * A080308  Non-multiples of Fermat numbers 2^(2^n)+1.
 * https://oeis.org/A080308
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n-th positive integer that is NOT divisible by any Fermat
   number F_k = 2^(2^k)+1.  It implements the definition of OEIS A080308.
   The sequence is the complement of the multiples of Fermat numbers.  */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* All Fermat numbers that fit in 64 bits (the ones that can affect
   numbers up to the order of 10^10, which covers the first 10000 terms
   of the sequence).  The array may be extended if larger Fermat numbers
   are ever needed. */
static const uint64_t _fermat_numbers[] = {
    3ULL,            /* F0 */
    5ULL,            /* F1 */
    17ULL,           /* F2 */
    257ULL,          /* F3 */
    65537ULL,        /* F4 */
    4294967297ULL    /* F5 */
};
static const size_t _fermat_cnt = sizeof(_fermat_numbers) / sizeof(_fermat_numbers[0]);

seq_t a(idx_t n)
{
    idx_t count = 0;              /* number of valid terms found so far */
    for (idx_t v = 1;; ++v) {     /* start from 1, as a(1)=1 */
        uint64_t sv = (uint64_t)v;    /* use a wider type for divisibility tests */
        bool is_multiple = false;
        for (size_t k = 0; k < _fermat_cnt; ++k) {
            if (sv % _fermat_numbers[k] == 0) {
                is_multiple = true;
                break;
            }
        }
        if (!is_multiple) {
            ++count;
            if (count == n) {
                return (seq_t)sv;   /* cast to seq_t; fits when result fits in seq_t */
            }
        }
    }
}
