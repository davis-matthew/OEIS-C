/*
 * A121515  Incorrect duplicate of A226508.
 * https://oeis.org/A121515
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(1)=3, a(2)=33, a(3)=315, a(4)=26163, a(5)=235953; For n ≥ 6:
 *            a(n) = 9·a(n−1) + 486·3^(n−5)
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 9, i64 n <= 19, i128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)3ull;
    if (n == 2) return (seq_t)33ull;
    if (n == 3) return (seq_t)315ull;
    if (n == 4) return (seq_t)26163ull;
    if (n == 5) return (seq_t)235953ull;

    seq_t term = (seq_t)235953ull;      // a(5)
    seq_t pow3 = (seq_t)1ull;           // 3^(5-5)

    for (idx_t k = 6; k <= n; ++k) {
        pow3 = pow3 * (seq_t)3ull;                 // 3^(k-5)
        seq_t diff = (seq_t)486ull * pow3;         // 486·3^(k-5)
        term = (seq_t)9ull * term + diff;          // recurrence
    }
    return term;
}
