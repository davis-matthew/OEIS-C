/*
 * A098155  Summarize the previous term in base 4 (in increasing order).
 * https://oeis.org/A098155
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 25, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A098155 – Summarize the previous term in base 4 (in increasing order)
   Recurrence:
     a(1) = 1
     for n > 1
        write a(n) by counting the occurrences of each base‑4 digit in a(n−1).
        For each digit d with non‑zero count, write the count in base‑4
        followed by the digit d, in ascending order of d.
   Example: a(8)=212223 → counts: 1:1, 2:4, 3:1 → "1 1 4 2 1 3" → a(9)=1110213.
   For n ≥ 12 the sequence stabilizes: a(n) = 101112213. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Immediate return for the fixed point reached at n = 12 */
    if (n >= 12U)
        return (seq_t)101112213U;

    /* Start with term a(1) */
    char cur[64] = "1";
    char nxt[64];

    for (idx_t step = 2U; step <= n; ++step) {
        unsigned count[4] = {0};

        /* Count digits 0–3 in cur */
        for (char *p = cur; *p; ++p)
            ++count[*p - '0'];

        /* Build the next term in ascending digit order */
        unsigned len = 0;
        for (int d = 0; d < 4; ++d) {
            if (!count[d])
                continue;

            /* Convert count[d] to base‑4 */
            unsigned val = count[d];
            char buf[20];
            int bi = 0;
            do {
                buf[bi++] = '0' + (val & 3U);
                val >>= 2U;
            } while (val);

            /* Reverse to correct order */
            for (int i = 0; i < bi / 2; ++i) {
                char tmp = buf[i];
                buf[i] = buf[bi - 1 - i];
                buf[bi - 1 - i] = tmp;
            }

            /* Append base‑4 count and the digit */
            for (int i = 0; i < bi; ++i)
                nxt[len++] = buf[i];
            nxt[len++] = '0' + d;
        }
        nxt[len] = '\0';

        /* Copy next term to cur for the next iteration */
        for (unsigned i = 0; i <= len; ++i)
            cur[i] = nxt[i];
    }

    /* Convert the final decimal‑digit string to seq_t */
    seq_t result = 0;
    for (unsigned i = 0; cur[i]; ++i)
        result = result * 10 + (cur[i] - '0');

    return result;
}
