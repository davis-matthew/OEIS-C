/*
 * A096087  Triangle read by rows: row n lists cubic remainders modulo n.
 * https://oeis.org/A096087
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 101, u16 n <= 101, u32 n <= 101, u64 n <= 101,
 *            u128 n <= 101, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A096087 -- triangle of distinct cubic residues modulo n, read rowwise */
#include <stdlib.h>
#include <stdbool.h>

/* Returns the n‑th term (n ≥ 2) of A096087. */
seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;          /* defined only for n ≥ 2 */
    idx_t target = n - 2;                /* zero‑based index in the sequence */

    idx_t pos = 0;                       /* number of elements produced so far */
    idx_t m = 2;                         /* current modulus (row) */

    /* temporary array to mark residues for each modulus */
    static bool *mark = NULL;
    static idx_t mark_cap = 0;

    for (;; ++m) {
        /* enlarge the mark array if needed */
        if (m > mark_cap) {
            void *tmp = realloc(mark, (size_t)m * sizeof(bool));
            bool *newp = (bool *)tmp;
            mark = newp;
            mark_cap = m;
        }
        /* clear marks */
        for (idx_t i = 0; i < m; ++i) mark[i] = false;

        idx_t cnt = 0;                   /* number of distinct residues for this m */
        for (idx_t k = 0; k < m; ++k) {
            /* compute k^3 mod m using seq_t arithmetic */
            seq_t kk = (seq_t)k;
            seq_t r  = ((kk * kk * kk) % (seq_t)m);
            size_t idx = (size_t)r;
            if (!mark[idx]) {
                mark[idx] = true;
                ++cnt;
            }
        }

        if (pos + cnt > target) {        /* target lies in this row */
            idx_t offset = target - pos; /* zero‑based offset within the row */
            idx_t count  = 0;
            for (idx_t r = 0; r < m; ++r) {
                if (mark[r]) {
                    if (count == offset) return (seq_t)r;
                    ++count;
                }
            }
            /* should never reach here */
            return (seq_t)0;
        }

        pos += cnt;                      /* advance past this row */
    }
}
