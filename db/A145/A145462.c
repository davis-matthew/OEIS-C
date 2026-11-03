/*
 * A145462  Eigentriangle, row sums = the Padovan sequence, A000931.
 * https://oeis.org/A145462
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  eigen, tabl, sign
 * Formula:   not stated
 * Range:     i8 n <= 83, i16 n <= 83, i32 n <= 83, i64 n <= 83, i128 n <= 83,
 *            big any n
 * Validated: partial, pass, 78 terms through n = 83 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Eigentriangle A145462 
   a(n) is the sequence obtained by reading the infinite lower‑triangle
   T(r,k)=C[r-k+1]·Pad(k) in row‑major order, starting with a(6)=T(1,1).
   Pad is the Padovan sequence starting with 1,1,2,2,3,4,5,7,… (offset
   6 in OEIS notation).  C is the periodic sequence 1,1,-1,0,1,-1,0,1,….
   The first index n=6 corresponds to r=1,k=1.
*/

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

/* The iterator for the Padovan numbers starts at 1,1,2. */
static seq_t *
ensure_pad_up_to(idx_t need)
{
    static seq_t *pad = NULL;
    static idx_t pad_len = 0;  /* number of values stored (0 means none) */

    if (pad_len == 0) {
        /* allocate space for at least 3 entries; index 0 unused */
        pad = mem_new(seq_t, (size_t)(4u));
        if (!pad) return NULL;
        pad_len = 3u;
        pad[1] = (seq_t)1;
        pad[2] = (seq_t)1;
        pad[3] = (seq_t)2;
    }
    if (need > pad_len) {
        size_t new_cap = (size_t)(need + 1u);
        seq_t *tmp = mem_resize(seq_t, pad, new_cap);
        if (!tmp) return NULL;
        pad = tmp;
        for (idx_t i = pad_len + 1u; i <= need; ++i) {
            /* Pad(k) = Pad(k-2)+Pad(k-3) for k>=4 */
            pad[i] = pad[i - 2] + pad[i - 3];
        }
        pad_len = need;
    }
    return pad;
}

/* coefficient C[t]  where t = r-k+1 >=1 */
static int
coeff(int t)
{
    if (t == 1 || t == 2) return 1;
    if (t == 3) return -1;
    /* sequence (starting t=4) : 0,1,-1,0,1,-1,...  period 3 */
    int mod = (t - 4) % 3;
    if (mod == 0) return 0;
    if (mod == 1) return 1;
    return -1;   /* mod == 2 */
}

seq_t a(idx_t n)
{
    if (n < 6u) return 0;          /* undefined for n<6 */

    /* m = offset from the first entry (index 6) */
    idx_t m = n - 6u;

    /* find row r such that r(r+1)/2 > m  */
    idx_t r = 1;
    while ((r * (r + 1u)) / 2u <= m) ++r;

    /* previous rows contain (r-1)r/2 terms */
    idx_t prev = (r - 1u) * r / 2u;
    /* column k (1‑based) */
    idx_t k = m - prev + 1u;

    /* Padovan number P(k) */
    seq_t *pad = ensure_pad_up_to(k);
    if (!pad) return 0;           /* allocation failed */

    /* coefficient C[r-k+1] */
    int c = coeff((int)(r - k + 1u));
    if (c == 0) return 0;

    return (seq_t)(c * pad[k]);
}
