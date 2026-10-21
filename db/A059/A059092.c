/*
 * A059092  Zeroless pandigitals expressible as a product of five factors that
 *          concatenate to another zeroless pandigital.
 * https://oeis.org/A059092
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, base, nonn
 * Formula:   a(n) is the nth smallest 9‑digit zeroless pandigital * number
 *            that can be written as a product of five integer factors * (each
 *            also zeroless) whose concatenation (in that factor order) *
 *            forms another 9‑digit zeroless pandigital
 * Range:     u8 none, u16 none, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* Forward declarations of helper functions */
static int next_permutation(char *a, int n);
static void ensure_seq_cap(size_t needed);
static void insertion_sort(void);

/* Global storage for the sequence */
static seq_t *seqs = NULL;    /* dynamic array of sequence values */
static size_t seqsz = 0;      /* number of stored values */
static size_t seqcap = 0;     /* allocated capacity */
static int initialized = 0;   /* flag to build sequence once */

/* Main function required by the repository */
seq_t a(idx_t n)
{
    if (!initialized) {
        /* Build the entire sequence once */
        char perm[9] = { '1', '2', '3', '4', '5', '6', '7', '8', '9' };
        unsigned int pow10[10];
        pow10[0] = 1;
        for (int i = 1; i <= 9; ++i) pow10[i] = pow10[i-1] * 10u;

        /* Generate all permutations of digits 1..9 */
        do {
            int numeric[10];
            numeric[0] = 0;
            for (int i = 1; i <= 9; ++i)
                numeric[i] = numeric[i-1] * 10 + (unsigned int)(perm[i-1] - '0');

            /* All ways to cut the 9‑digit permutation into 5 parts */
            for (int i1 = 1; i1 <= 5; ++i1)
                for (int i2 = i1 + 1; i2 <= 6; ++i2)
                    for (int i3 = i2 + 1; i3 <= 7; ++i3)
                        for (int i4 = i3 + 1; i4 <= 8; ++i4) {
                            unsigned int a1 = numeric[i1];
                            unsigned int a2 = numeric[i2] - numeric[i1] * pow10[i2 - i1];
                            unsigned int a3 = numeric[i3] - numeric[i2] * pow10[i3 - i2];
                            unsigned int a4 = numeric[i4] - numeric[i3] * pow10[i4 - i3];
                            unsigned int a5 = numeric[9] - numeric[i4] * pow10[9 - i4];

                            /* Multiply with early exit if product grows too large */
                            unsigned __int128 prod = 1;
                            prod *= a1;
                            if (prod > 987654321u) continue;
                            prod *= a2;
                            if (prod > 987654321u) continue;
                            prod *= a3;
                            if (prod > 987654321u) continue;
                            prod *= a4;
                            if (prod > 987654321u) continue;
                            prod *= a5;
                            if (prod > 987654321u) continue;

                            unsigned int p = (unsigned int)prod;

                            /* Check that p is a 9‑digit pandigital 1..9 */
                            unsigned mask = 0;
                            unsigned int tmp = p;
                            int ok = 1;
                            while (tmp > 0) {
                                unsigned d = tmp % 10;
                                if (d == 0) { ok = 0; break; }
                                if (mask & (1u << d)) { ok = 0; break; }
                                mask |= 1u << d;
                                tmp /= 10;
                            }
                            if (!ok) continue;
                            if (mask != 0x3FEu) continue; /* 0x3FE = bits 1..9 set */

                            /* Check for duplicates */
                            int dup = 0;
                            for (size_t k = 0; k < seqsz; ++k)
                                if (seqs[k] == (seq_t)p) { dup = 1; break; }
                            if (dup) continue;

                            /* Store unique product */
                            ensure_seq_cap(seqsz + 1);
                            seqs[seqsz++] = (seq_t)p;
                        }
        } while (next_permutation(perm, 9));

        /* Sort the sequence in ascending order */
        insertion_sort();

        initialized = 1;
    }

    if (n < 1 || (size_t)n > seqsz)
        return (seq_t)0;          /* undefined for n beyond the sequence size */
    return seqs[n-1];
}

/* Helper: generate next lexicographic permutation of array a[0..n-1] */
static int next_permutation(char *a, int n)
{
    int i = n - 2;
    while (i >= 0 && a[i] >= a[i+1]) --i;
    if (i < 0) return 0;
    int j = n - 1;
    while (a[j] <= a[i]) --j;
    /* swap a[i] and a[j] */
    char tmp = a[i];
    a[i] = a[j];
    a[j] = tmp;
    /* reverse from a[i+1] to a[n-1] */
    int l = i + 1, r = n - 1;
    while (l < r) {
        char t = a[l];
        a[l] = a[r];
        a[r] = t;
        ++l; --r;
    }
    return 1;
}

/* Ensure the sequence array has enough capacity */
static void ensure_seq_cap(size_t needed)
{
    if (needed <= seqcap) return;
    size_t newcap = seqcap ? seqcap * 2 : 16;
    if (newcap < needed) newcap = needed;
    seqs = (seq_t *)realloc(seqs, newcap * sizeof(seq_t));
    seqcap = newcap;
}

/* Simple insertion sort for the sequence array */
static void insertion_sort(void)
{
    for (size_t i = 1; i < seqsz; ++i) {
        seq_t key = seqs[i];
        size_t j = i;
        while (j > 0 && seqs[j-1] > key) {
            seqs[j] = seqs[j-1];
            --j;
        }
        seqs[j] = key;
    }
}
