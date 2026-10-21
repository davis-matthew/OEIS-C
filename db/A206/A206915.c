/*
 * A206915  The index (in A006995) of the greatest binary palindrome <= n;
 *          also the 'lower inverse' of A006995.
 * https://oeis.org/A206915
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of binary palindromes ≤ n (including 0 and 1) (the
 *            index of the greatest binary palindrome ≤ n)
 * Range:     u8 n <= 266, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation counts palindromes of each length L ≤ len(n)
   and for the current length uses a binary search on the left half. */

#include <stdint.h>
#include <stdbool.h>

/* Reverse the lowest `bits` bits of `x`.  Result is a basic integer type. */
static seq_t reverse_bits(idx_t x, idx_t bits)
{
    seq_t rev = 0;
    for (idx_t i = 0; i < bits; ++i) {
        rev = (rev << 1) | (x & 1);
        x >>= 1;
    }
    return rev;
}

/* Construct the binary palindrome of total length `L` from the left half `half`. */
static seq_t make_pal(idx_t half, idx_t L)
{
    idx_t half_len = (L + 1) / 2;
    if (L & 1) {                       /* odd length */
        seq_t left = ((seq_t)half) << (half_len - 1);
        seq_t rev  = reverse_bits(half >> 1, half_len - 1);
        return left | rev;
    } else {                            /* even length */
        seq_t left = ((seq_t)half) << half_len;
        seq_t rev  = reverse_bits(half, half_len);
        return left | rev;
    }
}

/* Return the OEIS sequence A206915: index of the greatest binary palindrome ≤ n. */
seq_t a(idx_t n)
{
    /* Special cases for 0 and 1 – match the OEIS offset. */
    if (n == 0) return 1;
    if (n == 1) return 2;

    /* Count of palindromes already known: 0 and 1. */
    seq_t count = 2;

    /* Length of the binary representation of n. */
    idx_t tmp = n;
    idx_t len_n = 0;
    while (tmp > 0) {
        len_n++;
        tmp >>= 1;
    }

    /* Add all palindromes of length < len_n. */
    for (idx_t L = 2; L < len_n; ++L) {
        idx_t half_len = (L + 1) / 2;
        count += ((seq_t)1 << (half_len - 1));
    }

    /* Count palindromes of the same length as n. */
    idx_t L = len_n;
    if (L >= 2) {
        idx_t half_len = (L + 1) / 2;
        idx_t start   = (idx_t)1 << (half_len - 1);   /* leading 1 */
        idx_t end     = ((idx_t)1 << half_len) - 1;   /* all possibilities */

        /* Binary search for the largest half producing a palindrome ≤ n. */
        idx_t low  = start, high = end, best = start - 1;
        while (low <= high) {
            idx_t mid = low + ((high - low) >> 1);
            seq_t pal = make_pal(mid, L);
            if (pal <= n) {
                best = mid;
                low  = mid + 1;
            } else {
                if (mid == 0) break;             /* safety for underflow */
                high = mid - 1;
            }
        }

        if (best >= start)
            count += (seq_t)(best - start + 1);
    }

    return count;
}
