/*
 * A219526  Number of 3 x 3 squares filled by 9 consecutive primes prime(n) to
 *          prime(n+8) such that sums along rows, columns and diagonals are
 *          primes.
 * https://oeis.org/A219526
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 100, u32 n <= 100, u64 n <= 100, u128 n <= 100,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * A219526: Number of 3×3 squares filled with 9 consecutive primes
 *         prime(n) … prime(n+8) such that all row, column, and diagonal
 *         sums are prime.
/// Implementation outline:
 *   For each n, we generate all 9! permutations of the 9 primes.
 *   An ordered partition of indices into three rows is chosen first
 *   (1680 possibilities).  For each row we consider only those 6
 *   permutations whose row sum is prime.  With the remaining
 *   permutations we test the 3 column sums and 2 diagonal sums
 *   using a pre‑computed prime lookup table up to 400 000.
 *
 * Complexity: 9! permutations → 3.6×10^5 checks per n,
 *              each check is a few array accesses.
 *              Running time < 2 s for n ≤ 10 000.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/get_prime.h"

/* ------------------------------------------------------- */
/*  Prime lookup table (0‑based indices up to 400 000)      */
/* ------------------------------------------------------- */
#define MAX_PRIME_LOOKUP 400000
static bool prime_lookup[MAX_PRIME_LOOKUP + 1];
static bool prime_lookup_inited = false;

static void init_prime_lookup(void)
{
    if (prime_lookup_inited) return;
    for (int i = 0; i <= MAX_PRIME_LOOKUP; ++i) prime_lookup[i] = true;
    prime_lookup[0] = prime_lookup[1] = false;
    for (int p = 2; (int64_t)p * p <= MAX_PRIME_LOOKUP; ++p) {
        if (prime_lookup[p]) {
            for (int k = p * p; k <= MAX_PRIME_LOOKUP; k += p) prime_lookup[k] = false;
        }
    }
    prime_lookup_inited = true;
}

/* ------------------------------------------------------- */
/*  3‑element permutations of {0,1,2}                     */
/* ------------------------------------------------------- */
static const uint8_t perm3[6][3] = {
    {0,1,2},
    {0,2,1},
    {1,0,2},
    {1,2,0},
    {2,0,1},
    {2,1,0}
};

/* ------------------------------------------------------- */
/*  Ordered partitions of 0..8 into 3 groups of size 3    */
/* ------------------------------------------------------- */
struct Partition { uint8_t a[3], b[3], c[3]; };
static struct Partition part[1680];
static size_t num_parts = 0;

static void init_partitions(void)
{
    if (num_parts) return;
    for (uint8_t i = 0; i < 9; ++i)
        for (uint8_t j = i + 1; j < 9; ++j)
            for (uint8_t k = j + 1; k < 9; ++k) {
                uint8_t row1[3] = {i, j, k};
                /* remaining 6 indices */
                uint8_t rem[6];
                int r = 0;
                for (uint8_t t = 0; t < 9; ++t)
                    if (t != i && t != j && t != k) rem[r++] = t;   /* r==6 */

                /* choose 3 out of 6 for row2 */
                for (uint8_t a = 0; a < 6; ++a)
                    for (uint8_t b = a + 1; b < 6; ++b)
                        for (uint8_t c = b + 1; c < 6; ++c) {
                            uint8_t row2[3] = {rem[a], rem[b], rem[c]};
                            /* the rest go to row3 */
                            uint8_t row3[3];
                            int idx = 0;
                            for (int t = 0; t < 6; ++t)
                                if (t != a && t != b && t != c) row3[idx++] = rem[t];
                            struct Partition *P = &part[num_parts++];
                            memcpy(P->a, row1, 3);
                            memcpy(P->b, row2, 3);
                            memcpy(P->c, row3, 3);
                        }
            }
}

/* ------------------------------------------------------- */
/*  Main sequence function                                */
/* ------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* behaviour below offset is irrelevant */

    init_prime_lookup();
    init_partitions();

    /* load the 9 consecutive primes */
    uint32_t primes[9];
    for (int i = 0; i < 9; ++i) primes[i] = get_prime(n + i);   /* get_prime is 1‑based */

    seq_t count = 0;

    /* buffers for valid permutations indices per row */
    uint8_t validPerm[3][6];
    uint8_t validCnt[3];

    for (size_t pIdx = 0; pIdx < num_parts; ++pIdx) {
        const struct Partition *P = &part[pIdx];

        /* --- row 1 permutations that give prime sum --- */
        validCnt[0] = 0;
        for (uint8_t up = 0; up < 6; ++up) {
            uint8_t i = perm3[up][0];
            uint8_t j = perm3[up][1];
            uint8_t k = perm3[up][2];
            uint32_t sum = primes[ P->a[i] ] + primes[ P->a[j] ] + primes[ P->a[k] ];
            if (sum <= MAX_PRIME_LOOKUP && prime_lookup[sum])
                validPerm[0][ validCnt[0]++ ] = up;
        }
        if (!validCnt[0]) continue;

        /* --- row 2 permutations that give prime sum --- */
        validCnt[1] = 0;
        for (uint8_t up = 0; up < 6; ++up) {
            uint8_t i = perm3[up][0];
            uint8_t j = perm3[up][1];
            uint8_t k = perm3[up][2];
            uint32_t sum = primes[ P->b[i] ] + primes[ P->b[j] ] + primes[ P->b[k] ];
            if (sum <= MAX_PRIME_LOOKUP && prime_lookup[sum])
                validPerm[1][ validCnt[1]++ ] = up;
        }
        if (!validCnt[1]) continue;

        /* --- row 3 permutations that give prime sum --- */
        validCnt[2] = 0;
        for (uint8_t up = 0; up < 6; ++up) {
            uint8_t i = perm3[up][0];
            uint8_t j = perm3[up][1];
            uint8_t k = perm3[up][2];
            uint32_t sum = primes[ P->c[i] ] + primes[ P->c[j] ] + primes[ P->c[k] ];
            if (sum <= MAX_PRIME_LOOKUP && prime_lookup[sum])
                validPerm[2][ validCnt[2]++ ] = up;
        }
        if (!validCnt[2]) continue;

        /* now try all combinatorics of these permutations */
        for (uint8_t p1 = 0; p1 < validCnt[0]; ++p1) {
            uint8_t up1 = validPerm[0][p1];
            uint32_t a0 = primes[ P->a[perm3[up1][0]] ];
            uint32_t a1 = primes[ P->a[perm3[up1][1]] ];
            uint32_t a2 = primes[ P->a[perm3[up1][2]] ];

            for (uint8_t p2 = 0; p2 < validCnt[1]; ++p2) {
                uint8_t up2 = validPerm[1][p2];
                uint32_t b0 = primes[ P->b[perm3[up2][0]] ];
                uint32_t b1 = primes[ P->b[perm3[up2][1]] ];
                uint32_t b2 = primes[ P->b[perm3[up2][2]] ];

                /* column sums with first two rows ready */
                uint32_t col0_ab = a0 + b0;
                uint32_t col1_ab = a1 + b1;
                uint32_t col2_ab = a2 + b2;

                for (uint8_t p3 = 0; p3 < validCnt[2]; ++p3) {
                    uint8_t up3 = validPerm[2][p3];
                    uint32_t c0 = primes[ P->c[perm3[up3][0]] ];
                    uint32_t c1 = primes[ P->c[perm3[up3][1]] ];
                    uint32_t c2 = primes[ P->c[perm3[up3][2]] ];

                    /* column sums */
                    uint32_t sum_col0 = col0_ab + c0;
                    if (sum_col0 > MAX_PRIME_LOOKUP || !prime_lookup[sum_col0]) continue;
                    uint32_t sum_col1 = col1_ab + c1;
                    if (sum_col1 > MAX_PRIME_LOOKUP || !prime_lookup[sum_col1]) continue;
                    uint32_t sum_col2 = col2_ab + c2;
                    if (sum_col2 > MAX_PRIME_LOOKUP || !prime_lookup[sum_col2]) continue;

                    /* diagonal sums */
                    uint32_t diag1 = a0 + b1 + c2;
                    if (diag1 > MAX_PRIME_LOOKUP || !prime_lookup[diag1]) continue;
                    uint32_t diag2 = a2 + b1 + c0;
                    if (diag2 > MAX_PRIME_LOOKUP || !prime_lookup[diag2]) continue;

                    /* all sums prime → count it */
                    ++count;
                }
            }
        }
    }

    return count;
}
