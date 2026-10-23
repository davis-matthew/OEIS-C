/*
 * A044919  Numbers whose base-3 run lengths form a palindrome.
 * https://oeis.org/A044919
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   is_run_palindrome
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) by brute‑force testing: for each candidate number,
   extract its base‑3 digits, collect the run lengths, and check
   whether that run‑length sequence is a palindrome. */ 

#include <stdbool.h>
#include "../utils/is_run_palindrome.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset starts at 1; behaviour below offset irrelevant */

    idx_t found = 0;
    seq_t candidate = 1;

    while (found < n) {
        if (is_run_palindrome(candidate)) {
            ++found;
            if (found == n) {
                return candidate;
            }
        }
        ++candidate;
    }
    return (seq_t)0;   /* unreachable */
}
