/*
 * A055391  Palindromic Lucas numbers (in the order of appearance).
 * https://oeis.org/A055391
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   is_palindrome
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Palindromic Lucas numbers: a(n) is the n-th Lucas number that is a
 * palindrome in base 10 (a(1)=L0=2, a(2)=L1=1, a(3)=L2=3, …). */

#include <stdbool.h>
#include <stddef.h>      /* for size_t, if needed */
#include "../utils/is_palindrome.h"   /* is_palindrome(seq_t x, idx_t base) */

seq_t a(idx_t n)
{
    /* For n <= 0 the behaviour is undefined; return 0. */
    if (n < 1) return (seq_t)0;

    idx_t count = 0;          /* how many palindromic Lucas numbers seen */
    seq_t prev   = 2;          /* L0  */
    seq_t curr   = 1;          /* L1  */
    idx_t idx    = 0;          /* current Lucas index, L0 for idx=0 */

    while (1)
    {
        seq_t val;
        if (idx == 0)          /* L0 */
        {
            val = prev;
        }
        else if (idx == 1)     /* L1 */
        {
            val = curr;
        }
        else                   /* Lk = Lk-1 + Lk-2 */
        {
            seq_t next = prev + curr;
            /* Detect unsigned overflow: wrap‑around makes result < operands. */
            if (next < curr) break;  /* further Lucas numbers cannot be represented */
            prev = curr;
            curr = next;
            val = curr;
        }

        /* Test for decimal palindrome. */
        if (is_palindrome(val, (idx_t)10))
        {
            ++count;
            if (count == n) return val;
        }

        ++idx;
        /* Practical safety: stop if we exceed a reasonable bound. */
        if (idx > 200000) break;
    }

    return (seq_t)0;   /* n-th palindromic Lucas number not found */
}
