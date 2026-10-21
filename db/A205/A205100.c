/*
 * A205100  The index j<k such that n divides k!!-j!!, where k is the least
 *          index (A204982) for which such j exists.
 * https://oeis.org/A205100
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = j where k is the smallest index such that n divides k!! –
 *            j!!, with 1 ≤ j < k. The function checks the residues of double
 *            factorials modulo n; the first repeated residue gives the
 *            required j
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n) {
    if (n == 0) /* undefined, but guard against bad input */
        return 0;

    /* Static buffer reused for all calls to avoid repeated allocations. */
    static idx_t *first_occ = NULL;
    static size_t first_size = 0;

    if (n > first_size) {
        void *tmp = realloc(first_occ, sizeof(idx_t) * n);
        if (!tmp)
            return 0;              /* allocation failed */
        first_occ = (idx_t *)tmp;
        first_size = n;
    }

    /* Reset the table: 0 means “not seen yet”. */
    memset(first_occ, 0, sizeof(idx_t) * n);

    seq_t odd_val = 1;   /* current odd double factorial modulo n */
    seq_t even_val = 1;  /* current even double factorial modulo n */

    for (idx_t k = 1; ; ++k) {
        seq_t val;
        if (k & 1) {          /* odd k : k!! = (k‑2)!! * k */
            val = (odd_val * (seq_t)k) % (seq_t)n;
            odd_val = val;
        } else {              /* even k : k!! = (k‑2)!! * k */
            val = (even_val * (seq_t)k) % (seq_t)n;
            even_val = val;
        }

        idx_t res = (idx_t)val;   /* residue modulo n, 0 ≤ res < n */
        idx_t j = first_occ[res];

        if (j != 0)              /* a repeat residue – k is the minimal one */
            return (seq_t)j;      /* j is the desired index */

        first_occ[res] = k;      /* record first occurrence of this residue */
    }
}
