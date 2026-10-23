/*
 * A144623  a(n) = A078822(n) - 1.
 * https://oeis.org/A144623
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct binary numbers occurring as proper
 *            substrings in the binary representation of n
 * Range:     u8 n <= 628, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count distinct binary substrings of n, then subtract 1 to
   exclude the whole string itself.  */
seq_t a(idx_t n)
{
    /* Special case: n = 0 has no proper substrings. */
    if (n == 0)
        return 0;

    /* Compute length L of the binary representation of n. */
    idx_t tmp = n;
    idx_t L = 0;
    while (tmp) {
        ++L;
        tmp >>= 1;
    }

    /* Maximum number of substrings for L<=64 is L*(L+1)/2 <= 2080. */
    seq_t seen[2100];
    idx_t distinct = 0;          /* number of distinct values seen */

    /* Enumerate all substrings.  */
    for (idx_t i = 0; i < L; ++i) {
        seq_t val = 0;
        for (idx_t j = i; j < L; ++j) {
            /* Extract the bit at position j (counting from the most
               significant bit as position 0). */
            idx_t bit = (n >> (L - 1 - j)) & 1;
            val = (val << 1) | bit;

            /* Check if this value has already been seen. */
            int found = 0;
            for (idx_t k = 0; k < distinct; ++k) {
                if (seen[k] == val) {
                    found = 1;
                    break;
                }
            }
            if (!found) {
                seen[distinct++] = val;
            }
        }
    }

    /* Subtract 1 to exclude the whole string itself.  */
    if (distinct == 0)
        return 0;
    return (seq_t)(distinct - 1);
}
