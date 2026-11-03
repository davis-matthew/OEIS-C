/*
 * A044414  Numbers n such that string 8,2 occurs in the base 10
 *          representation of n but not of n-1.
 * https://oeis.org/A044414
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Validated: partial, pass, 39 terms through n = 39 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044414: Numbers n such that the decimal string "82" occurs in n
   but not in n-1.  The function scans the decimal digits of each candidate
   and tests the required condition. */
#include <stdbool.h>
#include <stddef.h>

/* Return true if the decimal representation of n contains "82". */
static bool contains_82(seq_t n)
{
    if (n == 0) {
        return false;               /* "0" never contains "82" */
    }

    /* Buffer large enough for all 128‑bit numbers (< 40 decimal digits). */
    unsigned char digits[80];
    size_t len = 0;
    seq_t temp = n;
    const seq_t ten = (seq_t)10;

    /* Extract digits, least significant first. */
    while (temp > 0)
    {
        seq_t rem = temp % ten;
        /* Convert the remainder to an ordinary integer for storage. */
        digits[len++] = (unsigned char)((unsigned long long)rem);
        temp = temp / ten;
    }

    /* Scan from most significant to least significant digit looking for
       the pattern 8 followed by 2. */
    for (size_t i = len; i > 1; --i)
    {
        if (digits[i - 1] == 8 && digits[i - 2] == 2)
        {
            return true;
        }
    }
    return false;
}

/* Return the n‑th number such that "82" occurs in its base‑10 representation
   but does not occur in (n-1).  The index n starts at 1. */
seq_t a(idx_t n)
{
    seq_t candidate = 1;
    idx_t count = 0;

    while (count < n)
    {
        if (contains_82(candidate) && !contains_82(candidate - 1))
        {
            ++count;
            if (count == n)
                return candidate;
        }
        ++candidate;
    }

    /* This point is unreachable for valid inputs. */
    return (seq_t)0;
}
