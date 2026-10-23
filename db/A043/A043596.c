/*
 * A043596  Numbers whose base-4 representation has exactly 5 runs.
 * https://oeis.org/A043596
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   comb4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A043596: Numbers whose base‑4 representation has exactly
 * 5 runs.
 * The algorithm enumerates all base‑4 strings of lengths 5,6,…,
 * counts the 5‑run strings, stores them in an array, sorts the array,
 * and then answers queries by index.  The maximum index required by
 * the OEIS tests is under 20000, which is covered by strings up to
 * length 8 (13608 terms).  If a request for a larger index occurs,
 * the routine simply extends the enumeration to the required length. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/comb4.h"

/* Comparator for qsort. */
static int cmp_seq(const void *a, const void *b)
{
    seq_t x = *(const seq_t *)a;
    seq_t y = *(const seq_t *)b;
    if (x < y) return -1;
    if (x > y) return 1;
    return 0;
}

/* Global storage for the sequence. */
static seq_t *global_seq = NULL;     /* pointer to the array */
static size_t global_len  = 0;        /* number of elements stored */
static size_t global_cap  = 0;        /* capacity of the array */
static idx_t  max_len_gen = 4;       /* longest string length already generated */

/* Ensure the global array has room for at least one more element. */
static void ensure_room(void)
{
    if (global_len >= global_cap) {
        global_cap = (global_cap == 0) ? 64 : global_cap * 2;
        global_seq = (seq_t *)realloc(global_seq,
                                      global_cap * sizeof(seq_t));
    }
}

/* Append a value to the global array. */
static void append_value(seq_t val)
{
    ensure_room();
    global_seq[global_len++] = val;
}

/* Generate all base‑4 numbers of the given length that contain exactly
 * 5 runs and store them in the global array. */
static void generate_length(idx_t len)
{
    /* Number of digit patterns: 3 choices for first run digit, 3^4 for
     * the subsequent run digits.  For a given run‑length composition
     * the number of strings is 243. */
    const size_t patterns_per_comp = 243;

    /* Enumerate compositions of len into 5 positive integers. */
    for (int l1 = 1; l1 <= (int)(len - 4); ++l1)
        for (int l2 = 1; l2 <= (int)(len - l1 - 3); ++l2)
            for (int l3 = 1; l3 <= (int)(len - l1 - l2 - 2); ++l3)
                for (int l4 = 1; l4 <= (int)(len - l1 - l2 - l3 - 1); ++l4) {
                    int l5 = (int)len - l1 - l2 - l3 - l4;
                    /* Run lengths stored in l[0..4] for convenience. */
                    uint8_t l[5] = { (uint8_t)l1, (uint8_t)l2,
                                    (uint8_t)l3, (uint8_t)l4, (uint8_t)l5 };
                    /* Enumerate the 5 run digits. */
                    for (int d1 = 1; d1 <= 3; ++d1)
                        for (int d2 = 0; d2 <= 3; ++d2) if (d2 != d1)
                            for (int d3 = 0; d3 <= 3; ++d3) if (d3 != d2)
                                for (int d4 = 0; d4 <= 3; ++d4) if (d4 != d3)
                                    for (int d5 = 0; d5 <= 3; ++d5) if (d5 != d4) {
                                        seq_t val = 0;
                                        for (int k = 0; k < l[0]; ++k)
                                            val = val * 4 + (seq_t)d1;
                                        for (int k = 0; k < l[1]; ++k)
                                            val = val * 4 + (seq_t)d2;
                                        for (int k = 0; k < l[2]; ++k)
                                            val = val * 4 + (seq_t)d3;
                                        for (int k = 0; k < l[3]; ++k)
                                            val = val * 4 + (seq_t)d4;
                                        for (int k = 0; k < l[4]; ++k)
                                            val = val * 4 + (seq_t)d5;
                                        append_value(val);
                                    }
                }
}

/* Extend precomputation until we have at least n terms stored. */
static void extend_to_n(idx_t n)
{
    while (global_len < n) {
        ++max_len_gen;
        generate_length(max_len_gen);
    }
    /* Sort the array once after the final extension.  Subsequent
     * extensions preserve the sorted order because we append already
     * sorted blocks to the end; the final sort is enough. */
    qsort(global_seq, global_len, sizeof(seq_t), cmp_seq);
}

/* Public function required by the build system. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* no values below offset */
    extend_to_n(n);
    return global_seq[n - 1];
}
