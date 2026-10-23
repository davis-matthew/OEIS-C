/*
 * A170474  Number of reduced words of length n in Coxeter group on 33
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^45 = I.
 * https://oeis.org/A170474
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A170474
   Number of reduced words of length n in the Coxeter group on 33 generators
   with relations (S_i)^2 = (S_i S_j)^45 = I.
   Recurrence derived from the rational generating function:
   a(0)      = 1
   a(n)      = 31*(a(n‑1)+…+a(n‑44)) – 496*a(n‑45) + N(n)   (n ≥ 0)
   where N(0)=1, N(1…44)=2, N(45)=1 and N(n)=0 for n>45. */
#include <stddef.h>
#include "../utils/seq_alloc.h"   /* for seq_alloc and seq_realloc */
#include "../utils/seq_realloc.h"

seq_t a(idx_t n) {
    static seq_t *buf = NULL;   /* array of computed terms */
    static size_t cap = 0;      /* capacity of buf */
    static size_t size = 0;     /* number of terms stored (indices 0…size-1) */
    static seq_t sumPrev = 0;   /* sum of the last up to 44 terms */

    /* Ensure we have terms up to index n */
    if (size <= (size_t)n) {
        size_t need = (size_t)n + 1;
        if (cap < need) {
            size_t newcap = cap ? cap << 1 : 64;
            while (newcap < need) newcap <<= 1;
            buf = seq_realloc(buf, cap, newcap);
            cap = newcap;
        }
        while (size <= (size_t)n) {
            seq_t Nval;
            if (size == 0) {
                Nval = (seq_t)1;
            } else if (size == 45) {
                Nval = (seq_t)1;
            } else if (size <= 44) {
                Nval = (seq_t)2;
            } else {
                Nval = (seq_t)0;
            }

            seq_t sub = (seq_t)0;
            if (size >= 45) {
                sub = buf[size - 45] * (seq_t)496;
            }

            seq_t ak = Nval + (seq_t)31 * sumPrev - sub;
            buf[size] = ak;

            /* update running window sum for the next term */
            sumPrev += ak;
            if (size >= 45) {
                sumPrev -= buf[size - 44];   /* remove term leaving the 44‑term window */
            }

            ++size;
        }
    }
    return buf[(size_t)n];
}
