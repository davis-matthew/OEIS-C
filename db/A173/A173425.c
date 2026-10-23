/*
 * A173425  Ordered pair {vertex,genus} to the Csaszar equation:
 *          h=(V-3)*(V-4)/12.
 * https://oeis.org/A173425
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, uned
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 23, u32 n <= 23, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A173425: Ordered pair {vertex, genus} to the Csaszar equation
   (V-3)*(V-4)/12 = h.  The terms are interleaved:  a(2k)=V_k, a(2k+1)=h_k
   for the k‑th solution sorted by increasing vertex V.  The vertex and
   genus are stored exactly as integers that satisfy the equation.  This
   implementation generates solutions on-demand, caching them for
   efficiency.  It is valid C11 and C++17. */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* Internal cache for vertex (V) and genus (h) values. */
static seq_t *Varr = NULL;          /* vertex values */
static seq_t *Harr = NULL;          /* genus values */
static size_t cap   = 0;            /* current capacity of the arrays */
static size_t len   = 0;            /* number of stored pairs */
static idx_t lastV = 2;             /* last checked vertex (start before 3) */

/* Grow the cache arrays to accommodate at least 'needed' elements. */
static void ensure_capacity(size_t needed)
{
    if (cap >= needed)
        return;
    size_t newcap = cap == 0 ? 256 : cap * 2;
    while (newcap < needed)
        newcap *= 2;
    Varr = (seq_t *)realloc(Varr, newcap * sizeof(seq_t));
    Harr = (seq_t *)realloc(Harr, newcap * sizeof(seq_t));
    cap  = newcap;
}

/* Generate the next valid vertex/genus pair satisfying the Csaszar equation. */
static void generate_next(void)
{
    while (1) {
        ++lastV;                               /* consider next vertex */
        int64_t a = (int64_t)lastV - 3;         /* V-3  (may be negative for V<3) */
        int64_t b = (int64_t)lastV - 4;         /* V-4  */
        int64_t prod = a * b;                   /* (V-3)*(V-4) */
        if (prod % 12 == 0) {                   /* check divisibility */
            seq_t h = (seq_t)(prod / 12);       /* genus */
            ensure_capacity(len + 1);
            Varr[len] = lastV;
            Harr[len] = h;
            ++len;
            break;
        }
    }
}

/* Public function defined for the OEIS program. */
seq_t a(idx_t n)
{
    idx_t pair_index = n >> 1; /* floor(n/2) */

    /* Ensure the cache has at least pair_index + 1 entries. */
    while (len <= pair_index)
        generate_next();

    if ((n & 1ULL) == 0ULL)           /* even index: vertex */
        return Varr[pair_index];
    else                              /* odd index: genus   */
        return Harr[pair_index];
}
