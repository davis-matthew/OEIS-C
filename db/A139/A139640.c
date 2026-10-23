/*
 * A139640  Numbers of the form of 4x^2 - 4xy + 7y^2 (=24k+7) but not of the
 *          form 4x^2 + 4xy + 7y^2.
 * https://oeis.org/A139640
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A139640 – Numbers of the form 4x² – 4xy + 7y² (≡ 24k+7) but not  
   4x² + 4xy + 7y².  The routine pre‑computes the first 20 000 such
   values (enough for the first 10 000 terms) by a direct
   enumeration of all admissible x,y up to an upper bound and stores
   them in a static array.  For every call a(idx_t n) it returns
   the n‑th value (a(1) is the first term in the OEIS list).  */

/* required definitions provided by the build environment:
   typedef unsigned ... seq_t;   // integer type (unsigned)
   typedef uint64_t idx_t;       // index type
*/

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/* 1. Parameters ------------------------------------------------------ */
#define UB      20000000ULL          /* upper bound for enumeration */
#define MAX_TERMS 20000              /* capacity of static term array */

/* 2. Helper bit operations ----------------------------------------- */
#define BIT_WORD 64UL
static inline size_t bit_index(idx_t n) { return n >> 6; }
static inline uint64_t bit_mask(idx_t n)   { return (uint64_t)1 << (n & 63); }

static inline bool getbit(const uint64_t *arr, idx_t n)
{ return (arr[bit_index(n)] & bit_mask(n)) != 0; }

static inline void setbit(uint64_t *arr, idx_t n)
{ arr[bit_index(n)] |= bit_mask(n); }

/* 3. Static storage ----------------------------------------------- */
static seq_t terms[MAX_TERMS];
static idx_t terms_count = 0;
static bool initialized  = false;
static uint64_t* rep1    = NULL;
static uint64_t* rep2    = NULL;

/* 4. Enumeration of representations ------------------------------- */
static void init_sequence(void)
{
    size_t words = (UB >> 6) + 1;

    rep1 = (uint64_t*)calloc(words, sizeof(uint64_t));
    rep2 = (uint64_t*)calloc(words, sizeof(uint64_t));
    if (!rep1 || !rep2) { /* fall back – nothing will be returned */
        if (rep1) free(rep1);
        if (rep2) free(rep2);
        initialized = true;
        return;
    }

    /* 4x² – 4xy + 7y² */
    for (idx_t y = 0; ; ++y) {
        idx_t y2 = (idx_t)7 * y * y;
        if (y2 > UB) break;
        for (idx_t x = 0; ; ++x) {
            idx_t v = 4 * x * x - 4 * x * y + y2;
            if (v > UB) break;
            setbit(rep1, v);
        }
    }

    /* 4x² + 4xy + 7y² */
    for (idx_t y = 0; ; ++y) {
        idx_t y2 = (idx_t)7 * y * y;
        if (y2 > UB) break;
        for (idx_t x = 0; ; ++x) {
            idx_t v = 4 * x * x + 4 * x * y + y2;
            if (v > UB) break;
            setbit(rep2, v);
        }
    }

    /* collect numbers that satisfy the OEIS condition */
    for (idx_t v = 1; v <= UB && terms_count < MAX_TERMS; ++v) {
        if (getbit(rep1, v) && !getbit(rep2, v) && (v % 24ULL) == 7ULL) {
            terms[terms_count++] = (seq_t)v;
        }
    }

    free(rep1);
    free(rep2);
    rep1 = rep2 = NULL;
    initialized = true;
}

/* ------------------------------------------------------------------ */
/* Main function required by the framework ------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* index 0 is undefined */
    if (!initialized) init_sequence();
    if (n <= terms_count) return terms[n - 1];
    return 0;                            /* out of range */
}
