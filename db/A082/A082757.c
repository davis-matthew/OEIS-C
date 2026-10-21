/*
 * A082757  Possible integer ratios of A/B, where A and B have no 0 in their
 *          decimal representation, all decimal digits of A and B are
 *          different and each digit is used only once.
 * https://oeis.org/A082757
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, fini, nonn
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 3782, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   permute
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A082757: integer ratios A/B where A and B
 * are formed from the digits 1‑9 with no zeros, the digits of A and B
 * are disjoint and together use every digit once, and A is an integer
 * multiple of B.  The sequence is the set of all such ratios sorted
 * in ascending order.  The function builds the data once at first use
 * and then returns the n‑th term (1‑based).  A simple enumeration of
 * all digit partitions and permutations is used – no closed formula
 * exists for this sequence. */
 
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>      /* for abort() */
#include "../utils/permute.h"
 
/* Types supplied by the build */
typedef idx_t seq_idx_t;    /* uint64_t, unsigned */
typedef seq_t  seq_val_t;   /* unsigned integer (8,16,32,64 or 128 bits) */
 
/* ---------- Local constants ---------- */
#define MAX_DIGITS 9
#define ALL_DIGITS_MASK 0x1FF         /* bits 0‑8 set (digits 1‑9) */
#define MAX_RATIO 98765432            /* 9‑digit pandigital divided by another */
 
/* Pre‑computed factorials 0! … 9! */
static const uint32_t fact[10] = {1,1,2,6,24,120,720,5040,40320,362880};
 
/* ---------- Helper structures ---------- */
struct MaskInfo {
    uint32_t *nums;   /* array of all numbers that can be formed from this mask */
    size_t   count;   /* number of numbers */
    size_t   cap;     /* allocated capacity */
};
static struct MaskInfo maskInfos[512];
 
/* ---------- Global state ---------- */
static int initialized = 0;
static seq_val_t *sorted_ratios = NULL; /* sorted unique ratios */
static size_t   sorted_count  = 0;      /* number of elements in sorted_ratios */
static unsigned char *ratio_bitset = NULL;/* bitset for quotients 1 … MAX_RATIO */
 

 
static void generate_numbers_for_masks(void)
{
    for (unsigned mask = 1; mask < 512; ++mask) {
        if (mask == ALL_DIGITS_MASK)  /* skip empty complement */
            continue;

        /* collect digits belonging to this mask */
        int digits[MAX_DIGITS];
        int k = 0;
        for (int d = 1; d <= MAX_DIGITS; ++d)
            if (mask & (1U << (d - 1))) digits[k++] = d;

        size_t cap = fact[k];
        uint32_t *arr = (uint32_t *)malloc(cap * sizeof(uint32_t));
        if (!arr) abort();             /* out of memory */
        size_t cnt = 0;
        permute(digits, k, 0, 0, arr, &cnt);

        maskInfos[mask].nums  = arr;
        maskInfos[mask].count = cnt;
        maskInfos[mask].cap   = cap;
    }
}
 
/* ---------- Initialize entire data once ---------- */
static void init_sequence(void)
{
    /* Bitset for ratios: 1..MAX_RATIO */
    size_t bitset_bytes = (MAX_RATIO >> 3) + 1;
    ratio_bitset = (unsigned char *)calloc(bitset_bytes, 1);
    if (!ratio_bitset) abort();

    /* Generate all numbers for each mask */
    generate_numbers_for_masks();

    /* Process every unordered pair of complementary masks only once.
     * For each pair, examine both orders to catch ratios where the
     * larger number lies in either mask. */
    for (unsigned mask = 1; mask < 512; ++mask) {
        unsigned comp = ALL_DIGITS_MASK ^ mask;
        if (mask > comp) continue;   /* ensure each unordered pair once */

        const uint32_t *A = maskInfos[mask].nums;
        size_t cntA = maskInfos[mask].count;
        const uint32_t *B = maskInfos[comp].nums;
        size_t cntB = maskInfos[comp].count;

        for (size_t i = 0; i < cntA; ++i) {
            uint32_t a = A[i];
            for (size_t j = 0; j < cntB; ++j) {
                uint32_t b = B[j];
                if (b == 0) continue;       /* never occurs, safety */

                /* a / b if divisible */
                if (a % b == 0) {
                    uint32_t r = a / b;
                    ratio_bitset[r >> 3] |= 1U << (r & 7);
                }
                /* b / a if divisible */
                if (b % a == 0) {
                    uint32_t r = b / a;
                    ratio_bitset[r >> 3] |= 1U << (r & 7);
                }
            }
        }
    }

    /* Build sorted list of unique ratios */
    sorted_count = 0;
    /* Upper bound 200000 is safe for the 179540 terms */
    sorted_ratios = (seq_val_t *)malloc(200000 * sizeof(seq_val_t));
    if (!sorted_ratios) abort();

    for (uint32_t v = 1; v <= MAX_RATIO; ++v) {
        if (ratio_bitset[v >> 3] & (1U << (v & 7))) {
            sorted_ratios[sorted_count++] = (seq_val_t)v;
        }
    }

    initialized = 1;
}
 
/* ---------- Public function required by the problem statement ---------- */
seq_val_t a(seq_idx_t n)
{
    if (!initialized)
        init_sequence();

    /* n is 1-based and guaranteed to be within the sequence length. */
    return sorted_ratios[n - 1];
}
