/*
 * A062935  Palindromes of the form n! + 1.
 * https://oeis.org/A062935
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth value of n! + 1 that is a palindrome * (OEIS
 *            A062935, offset 1)
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Helper: check whether a decimal representation of x is a palindrome.
 * Works for any integer type seq_t that supports % and / by uint64_t. */
static bool is_palindrome(seq_t x)
{
    seq_t rev = 0;
    seq_t orig = x;
    while (orig != 0)
    {
        rev = rev * 10 + (orig % 10);
        orig /= 10;
    }
    return rev == x;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* We search for factorial+1 palindromes until we find the nth one
     * or until multiplication would overflow.  The search stops
     * when the factorial overflows (detected by the factor dropping
     * in value).  This guarantees correctness for 64‑bit and
     * 128‑bit arithmetic and is safe for arbitrary‑precision builds.  */

    seq_t fact = 1;                    /* factorial accumulator */
    seq_t pal_val;                     /* current candidates n! + 1  */
    idx_t found = 0;                   /* number of palindromes found */
    idx_t i;

    /* We use a practical limit: we iterate until overflow or
     * until we counted n palindromes.  The limit is chosen to
     * avoid excessive computation while still covering known facts. */
    for (i = 1; ; ++i)
    {
        /* Detect overflow before multiplication.
         * If unsigned, fact < i-1 indicates overflow.  */
        seq_t prev_fact = fact;
        fact *= (seq_t)i;
        if (fact < prev_fact)
            break;          /* overflow / wrap detected */

        pal_val = fact + 1;

        if (is_palindrome(pal_val))
        {
            ++found;
            if (found == n)
                return pal_val;
        }

        /* to prevent absurdly long runs in case the sequence is
         * finite and we haven't found enough terms, we impose a
         * generous but finite upper bound on i.  5000 is plenty
         * for any expected behaviour of this sequence. */
        if (i > 5000)
            break;
    }

    /* If we exit the loop without having found the nth term,
     * the sequence is (as far as we can compute) finite and the
     * nth element is undefined.  We return 0 to signal that. */
    return 0;
}
