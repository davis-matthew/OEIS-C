/*
 * A229499  Palindromic prime numbers == 4 (mod 9).
 * https://oeis.org/A229499
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 11, u32 n <= 987, u64 n <= 987, u128 n <= 987,
 *            big any n
 * Helpers:   isPrimeGeneric, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: Enumerates palindromic primes that are ≡ 4 (mod 9) and
   returns the n‑th such prime (a(1) is the first). */

#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"
#include "../utils/pow10.h"

/* Build a decimal palindrome from the left half 'left'.
   If 'odd' is true the central digit is not duplicated. */
static seq_t
make_palindrome(seq_t left, bool odd)
{
    seq_t pal = left;
    seq_t r   = odd ? left / (seq_t)10 : left;
    while (r > (seq_t)0) {
        pal = pal * (seq_t)10 + (r % (seq_t)10);
        r   = r / (seq_t)10;
    }
    return pal;
}

/* Return the n‑th palindromic prime that is ≡ 4 (mod 9).  The function
   is memoised implicitly by the loop; it does not rely on global state
   and works for any sequence of calls. */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;     /* undefined for n < 1 */

    idx_t  count = (idx_t)0;                /* how many qualifying numbers found */
    idx_t  len   = (idx_t)1;                /* current palindrome length */

    while (true) {
        idx_t  half_len = (len + (idx_t)1) / (idx_t)2;   /* digits in the left half */
        seq_t  start    = pow10(half_len - (idx_t)1);    /* first half with leading digit ≠ 0 */
        seq_t  end      = start * (seq_t)10 - (seq_t)1;  /* last half of this length */
        bool   odd      = (len % (idx_t)2 == (idx_t)1);

        for (seq_t left = start; left <= end; left = left + (seq_t)1) {
            seq_t pal = make_palindrome(left, odd);

            if ((pal % (seq_t)9) == (seq_t)4 && isPrimeGeneric(pal)) {
                ++count;
                if (count == n)
                    return pal;
            }
        }
        ++len;    /* move to palindromes of next length */
    }
}
