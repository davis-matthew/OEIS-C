/*
 * A043571  Numbers whose base-2 representation has exactly 4 runs.
 * https://oeis.org/A043571
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n‑th smallest integer whose binary representation *
 *            consists of exactly four consecutive runs of equal bits. The
 *            runs have * lengths a,b,c,d ≥ 1 and the binary form is 1^a 0^b
 *            1^c 0^d. * The sequence contains only even numbers
 * Range:     u8 n <= 69, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Comparison routine for qsort – works for any unsigned seq_t type */
static int cmp_seq(const void *p1, const void *p2)
{
    const seq_t a = *(const seq_t *)p1;
    const seq_t b = *(const seq_t *)p2;
    if (a < b) return -1;
    if (a > b) return 1;
    return 0;
}

/* Allocate and free an array of seq_t – works in C and C++ */
#ifdef __cplusplus
static seq_t *allocSeq(size_t n) { return new seq_t[n]; }
static void freeSeq(seq_t *p)   { delete[] p; }
#else
static seq_t *allocSeq(size_t n) { return (seq_t*)malloc(n * sizeof(seq_t)); }
static void freeSeq(seq_t *p)   { free(p); }
#endif

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* defensive – not used for the sequence */

    /* Find the smallest binary length L such that the total number of
     * numbers with length ≤ L is at least n.  The number of compositions
     * of len into 4 positive parts is C(len-1,3).
     */
    int L = 4;
    size_t cum = 0;
    while (cum < (size_t)n) {
        ++L;
        size_t cnt = (size_t)((L - 1) * (L - 2) * (L - 3) / 6);
        cum += cnt;
    }

    /* Compute the exact total number of candidates up to this L. */
    size_t total = 0;
    for (int len = 4; len <= L; ++len)
        total += (size_t)((len - 1) * (len - 2) * (len - 3) / 6);

    /* Enumerate all 4‑run numbers with binary length ≤ L. */
    seq_t *arr = allocSeq(total);
    size_t pos = 0;
    for (int len = 4; len <= L; ++len) {
        for (int a = 1; a <= len - 3; ++a) {
            for (int c = 1; c <= len - a - 2; ++c) {
                for (int d = 1; d <= len - a - c - 1; ++d) {
                    int b = len - a - c - d;  /* b ≥ 1 by construction */

                    /* Build the integer: 1^a 0^b 1^c 0^d */
                    seq_t part1 = ((seq_t)1 << a) - 1;
                    part1 <<= (b + c + d);
                    seq_t part2 = ((seq_t)1 << c) - 1;
                    part2 <<= d;
                    arr[pos++] = part1 + part2;
                }
            }
        }
    }

    /* Sort the gathered numbers and return the (n‑1)th element. */
    qsort(arr, total, sizeof(seq_t), cmp_seq);
    seq_t result = arr[n - 1];
    freeSeq(arr);
    return result;
}
