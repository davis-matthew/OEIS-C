/*
 * A164455  Number of binary strings of length n with no substrings equal to
 *          0001 0011 or 0111.
 * https://oeis.org/A164455
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 23, u32 n <= 50, u64 n <= 102,
 *            u128 n <= 208, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = number of binary strings of length n with no
   substrings equal to 0001, 0011, or 0111.
   The function uses a finite‑automaton DP over the last
   three bits.  For lengths < 4 all extensions are allowed, because
   a forbidden 4‑bit window cannot yet be formed.  For i ≥ 3 the
   transition to a state that would create 0001 (1), 0011 (3), or
   0111 (7) is rejected.  The total a(n) is the sum of all state
   counts after processing n bits.  This works for all n ≥ 4. */

seq_t a(idx_t n)
{
    /* DP arrays for the current and next length, indexed by the
       three‑bit suffix (values 0..7). */
    seq_t cur[8] = {0};
    seq_t nxt[8] = {0};

    /* empty string has no suffix; use suffix 0 as an arbitrary start. */
    cur[0] = 1;

    for (idx_t i = 0; i < n; ++i) {
        /* zero the next array */
        for (int j = 0; j < 8; ++j)
            nxt[j] = 0;

        /* transition from every current state */
        for (int s = 0; s < 8; ++s) {
            seq_t ways = cur[s];
            if (!ways)
                continue;

            /* try appending 0 and 1 */
            for (int b = 0; b <= 1; ++b) {
                int pat = (s << 1) | b;          /* 4‑bit window */
                if (i >= 3 && (pat == 1 || pat == 3 || pat == 7))
                    continue;                    /* forbidden pattern */

                int ns = pat & 7;                /* new 3‑bit suffix */
                nxt[ns] += ways;
            }
        }

        /* move to the next length */
        for (int j = 0; j < 8; ++j)
            cur[j] = nxt[j];
    }

    /* sum over all suffix states to get the total number of strings
       of length n */
    seq_t total = 0;
    for (int j = 0; j < 8; ++j)
        total += cur[j];

    return total;
}
