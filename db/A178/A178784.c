/*
 * A178784  Let d be the vector of divisors of 100 sorted from largest to
 *          smallest, i.e., [100,50,25,20,10,5,4,2,1]. Then a(n) = 100/d(n) -
 *          1.
 * https://oeis.org/A178784
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = 100/d(n) - 1, with d(n) = 100/(n-th divisor of 100 in
 *            descending order), repeated periodically
 * Range:     u8 n <= 9, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
seq_t a(idx_t n)
{
    static const seq_t divs[] = {
        (seq_t)100, (seq_t)50, (seq_t)25, (seq_t)20,
        (seq_t)10, (seq_t)5,  (seq_t)4,  (seq_t)2,
        (seq_t)1
    };
    const size_t len = sizeof(divs) / sizeof(divs[0]);

    /* n is 1‑based; behaviour for n == 0 is undefined, return 0 for safety */
    if (n == (idx_t)0)
        return (seq_t)0;

    idx_t idx = (n - (idx_t)1) % (idx_t)len;
    seq_t d = divs[(size_t)idx];
    return (seq_t)((seq_t)100 / d - (seq_t)1);
}
