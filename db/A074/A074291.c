/*
 * A074291  Positions in the Kolakoski sequence where A000002(n) is 1 if n is
 *          odd, or 2 if n is even.
 * https://oeis.org/A074291
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth index i such that the Kolakoski sequence
 *            A000002(i) equals 1 if i is odd, and 2 if i is even. The
 *            algorithm generates the Kolakoski sequence on the fly and
 *            collects the matching positions
 * Range:     u8 n <= 131, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

/* External types supplied by the build.  Do not modify. */
extern seq_t a(idx_t n);

seq_t a(idx_t n)
{
    /* Static state preserved between calls. */
    static seq_t *seq = NULL;          /* generated Kolakoski terms (1‑based)   */
    static size_t seq_cap = 0;         /* capacity of seq array                */
    static idx_t seq_len = 0;          /* number of terms currently stored     */

    static seq_t *matches = NULL;      /* indices where parity matches         */
    static size_t matches_cap = 0;     /* capacity of matches array            */
    static idx_t match_count = 0;      /* number of matches currently stored   */

    static idx_t run_ptr = 1;          /* index of current run length (1‑based)*/
    static idx_t pos = 2;              /* next index to generate (1‑based)     */
    static seq_t current_val = 1;      /* value of the current run (1 or 2)    */
    static idx_t count = 1;            /* elements already output in current run*/
    static bool initialized = false;   /* one‑time initialization flag         */

    /* One‑time initialization. */
    if (!initialized) {
        /* Allocate initial capacity. */
        seq_cap = 256;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        if (!seq) return 0;
        seq[0] = 1;           /* a[1] = 1 */
        seq_len = 1;

        matches_cap = 256;
        matches = (seq_t *)malloc(matches_cap * sizeof(seq_t));
        if (!matches) return 0;

        /* Position 1 always matches the parity rule (1 at odd index). */
        matches[match_count++] = (seq_t)1;

        initialized = true;
    }

    /* Generate terms until we have at least n matches. */
    while ((idx_t)match_count < n) {
        /* Check if the current run has finished. */
        if (count == seq[(run_ptr - 1)]) {
            run_ptr++;
            current_val = 3 - current_val;   /* toggle between 1 and 2 */
            count = 0;
        }

        /* Grow seq array if necessary. */
        if ((idx_t)pos > seq_cap) {
            size_t new_cap = seq_cap * 2;
            seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
            if (!tmp) return 0;
            seq = tmp;
            seq_cap = new_cap;
        }

        /* Generate the next term. */
        seq[(pos - 1)] = current_val;
        seq_len = pos;

        /* Check parity rule: expected 1 if pos odd, else 2. */
        seq_t expected = (pos & 1) ? 1 : 2;
        if (expected == current_val) {
            /* Grow matches array if necessary. */
            if ((idx_t)match_count >= matches_cap) {
                size_t new_cap = matches_cap * 2;
                seq_t *tmp = (seq_t *)realloc(matches, new_cap * sizeof(seq_t));
                if (!tmp) return 0;
                matches = tmp;
                matches_cap = new_cap;
            }
            matches[match_count++] = (seq_t)pos;
        }

        pos++;
        count++;
    }

    return matches[n - 1];
}
