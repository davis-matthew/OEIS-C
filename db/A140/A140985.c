/*
 * A140985  First row of infinite array A(j,k): A(j,1) = j-1; A(1,k) =
 *          A(2,k-1); for j, k > 1, A(j,k) = A(j-1,k) - A(j+1,k-1) if that
 *          number is positive and not already in column k, A(j,k) = A(j-1,k)
 *          + A(j+1,k-1) otherwise.
 * https://oeis.org/A140985
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the first row of the infinite array A(j,k) defined by *
 *            A(j,1) = j-1, * A(1,k) = A(2,k-1), * A(j,k) = A(j-1,k) -
 *            A(j+1,k-1) if the number is positive and * not already in column
 *            k; * otherwise A(j,k) = A(j-1,k) + A(j+1,k-1). The function
 *            returns * a(n)=A(1,n), with indexing starting at 1....
 * Range:     u8 n <= 11, u16 n <= 21, u32 n <= 53, u64 n <= 109,
 *            u128 n <= 217, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include the standard headers required for the implementation. */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* --------------------------------------------------------------------- */
/* Simple hash function for a seq_t value.  The function works for all
 * of the types that can be used as seq_t: unsigned integer types,
 * unsigned __int128, and the arbitrary‑precision integer class used in
 * the C++ builds.  For the integer types it reduces the value modulo a
 * large 32‑bit prime.  For the arbitrary‑precision class the builtin
 * modulo operator is used.  The result is cast to uint64_t which is
 * safe for use as an index. */
static inline uint64_t seq_hash(seq_t v)
{
    /* 0x9E3779B9 is the golden‑ratio constant. */
    return (uint64_t)(v % (seq_t)0x9E3779B9U);
}

/* --------------------------------------------------------------------- */
/* Return the next power of two greater than or equal to x.  The function
 * is used to size the hash table for each column. */
static inline size_t next_pow2(size_t x)
{
    size_t p = 1;
    while (p < x) p <<= 1;
    return p;
}

/* --------------------------------------------------------------------- */
/* Helper macros to allocate and free arrays that may contain non‑trivial
 * objects in the C++ builds.  For C builds the standard malloc/free
 * functions are used. */
#ifdef __cplusplus
#define ALLOC_SEQ(n)  new seq_t[(n)]
#define FREE_SEQ(p)   delete[] (p)
#else
#define ALLOC_SEQ(n)  ((seq_t*)malloc((n) * sizeof(seq_t)))
#define FREE_SEQ(p)   free(p)
#endif

/* Helper macros for the boolean array used in the hash table. */
#ifdef __cplusplus
#define ALLOC_UBYTE(n)  new unsigned char[(n)]()
#define FREE_UBYTE(p)   delete[] (p)
#else
#define ALLOC_UBYTE(n)  ((unsigned char*)calloc((n), 1))
#define FREE_UBYTE(p)   free(p)
#endif

/* --------------------------------------------------------------------- */
/* Main function:  a(n) calculates A(1,n).  The algorithm keeps only the
 * previous column and the current column in memory; a hash table for
 * each column provides O(1) membership tests for the Recamán–style
 * condition. */
seq_t a(idx_t n)
{
    if (n <= 1)          /*  a(1) = 0  */
        return (seq_t)0;

    /* ===================== Column 1 ===================== */
    /* Two elements more than needed are allocated to allow the
     * “prev[j+1]” access used later (column 2 needs prev[2]). */
    seq_t *prev = ALLOC_SEQ((size_t)n + 2);
    for (idx_t j = 1; j <= n; ++j)
        prev[j] = j - 1;          /*  A(j,1) = j-1  */

    /* ===================== Subsequent columns ===================== */
    for (idx_t k = 2; k <= n; ++k)
    {
        size_t max_j = (size_t)(n - k + 1);          /* j ranges 1 … max_j */
        seq_t *cur    = ALLOC_SEQ(max_j + 2);        /* indices 1 … max_j */

        /* ------------- Hash table for column k ------------- */
        size_t tbl_size = next_pow2(((max_j) << 1) + 3);
        seq_t *hash_tbl = ALLOC_SEQ(tbl_size);
        unsigned char *used_tbl = ALLOC_UBYTE(tbl_size);
        size_t mask = tbl_size - 1;                 /* tbl_size is a power of two */

        /* ------------  A(1,k)  = A(2,k-1)  ------------- */
        cur[1] = prev[2];
        size_t h = seq_hash(cur[1]) & mask;
        while (used_tbl[h]) h = (h + 1) & mask;
        used_tbl[h] = 1;
        hash_tbl[h] = cur[1];

        /* ------------ Fill the rest of column k ------------- */
        for (size_t j = 2; j <= max_j; ++j)
        {
            seq_t up   = cur[j - 1];
            seq_t down = prev[j + 1];
            seq_t val;
            bool use_diff = (up > down);                /* up - down > 0?  */

            if (use_diff)                              /* candidate = up - down  */
            {
                seq_t x = up - down;
                size_t h2 = seq_hash(x) & mask;
                while (used_tbl[h2] && hash_tbl[h2] != x)
                    h2 = (h2 + 1) & mask;
                use_diff = !used_tbl[h2];              /* true if x not in column */
                if (use_diff) { val = x; }
            }

            if (!use_diff)                              /* otherwise use sum  */
                val = up + down;

            cur[j] = val;

            size_t h3 = seq_hash(val) & mask;
            while (used_tbl[h3]) h3 = (h3 + 1) & mask;
            used_tbl[h3] = 1;
            hash_tbl[h3] = val;
        }

        /* -------------------  Return if finished  -------------------- */
        if (k == n)
        {
            seq_t result = cur[1];
            FREE_SEQ(cur);
            FREE_UBYTE(used_tbl);
            FREE_SEQ(hash_tbl);
            FREE_SEQ(prev);
            return result;
        }

        /* -------------------  Prepare for next iteration  -------------------- */
        FREE_SEQ(prev);
        FREE_UBYTE(used_tbl);
        FREE_SEQ(hash_tbl);
        prev = cur;                                 /* prev is now column k */
    }

    /* Should never reach here, but keep compiler happy. */
    return (seq_t)0;
}
