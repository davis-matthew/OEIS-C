/*
 * A023501  a(n) = b(n) + d(n), where b(n) = (n-th Lucas number > 1) and d(n)
 *          = (n-th number that is 1, 2, or 3, or is not a Lucas number).
 * https://oeis.org/A023501
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = b(n) + d(n); b(n) = (n+1)-th Lucas number with L0 = 1, L1
 *            = 3 (Lucas numbers > 1); d(n) = n-th positive integer that is 1,
 *            2, 3, or not a Lucas number
 * Range:     u8 n <= 9, u16 n <= 21, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static seq_t *lucas = NULL;          /* Lucas numbers starting at L0=1, L1=3 */
    static idx_t lucas_len = 0;          /* number of computed Lucas terms          */
    static size_t lucas_cap = 0;         /* allocated capacity for Lucas array       */

    static seq_t *allowed = NULL;        /* allowed numbers list                    */
    static idx_t allowed_len = 0;        /* number of computed allowed terms        */
    static size_t allowed_cap = 0;       /* allocated capacity for allowed array    */

    /* Persisted state for efficient enumeration of allowed terms */
    static seq_t next_cur = (seq_t)1;    /* next integer to consider                */
    static idx_t lucas_idx = 0;          /* index into lucas array for comparison  */

    /* ---------- Ensure Lucas numbers up to index n ---------- */
    if (lucas_len <= n) {
        /* Grow capacity if necessary */
        if (lucas_cap < (size_t)(n + 1)) {
            size_t new_cap = lucas_cap == 0 ? 4 : lucas_cap * 2;
            while (new_cap < (size_t)(n + 1))
                new_cap *= 2;
            lucas = (seq_t *)realloc(lucas, new_cap * sizeof(seq_t));
            lucas_cap = new_cap;
        }
        /* Initialize first two Lucas numbers (L0=1, L1=3) */
        if (lucas_len == 0) {
            lucas[0] = (seq_t)3;   /* corresponds to L1 = 3                     */
            lucas_len = 1;
        }
        if (lucas_len == 1) {
            lucas[1] = (seq_t)4;   /* corresponds to L2 = 4                     */
            lucas_len = 2;
        }
        /* Generate remaining Lucas numbers up to index n */
        while (lucas_len <= n) {
            lucas[lucas_len] = lucas[lucas_len-1] + lucas[lucas_len-2];
            lucas_len++;
        }
    }

    /* ---------- Ensure allowed numbers up to index n ---------- */
    if (allowed_len <= n) {
        /* Grow capacity if necessary */
        if (allowed_cap < (size_t)(n + 1)) {
            size_t new_cap = allowed_cap == 0 ? 4 : allowed_cap * 2;
            while (new_cap < (size_t)(n + 1))
                new_cap *= 2;
            allowed = (seq_t *)realloc(allowed, new_cap * sizeof(seq_t));
            allowed_cap = new_cap;
        }
        /* Enumerate integers starting from next_cur until we have n+1 allowed terms */
        while (allowed_len <= n) {
            seq_t cur = next_cur;

            /* Advance lucas_idx so that lucas[lucas_idx] >= cur (if possible) */
            while (lucas_idx < lucas_len && lucas[lucas_idx] < cur)
                lucas_idx++;

            bool is_lucas = (lucas_idx < lucas_len && lucas[lucas_idx] == cur);

            if (cur <= (seq_t)3 || !is_lucas) {
                allowed[allowed_len++] = cur;
            }

            next_cur = cur + (seq_t)1;
        }
    }

    /* Return the sum of the required Lucas and allowed terms */
    return lucas[n] + allowed[n];
}
