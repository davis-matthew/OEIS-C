/*
 * A210016  In base 5, numbers n which have 5 distinct digits, do not start
 *          with 0, and have property that the product (written in base 5) of
 *          any two adjacent digits is a substring of n.
 * https://oeis.org/A210016
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 18, u32 n <= 18, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   prod_base5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: generate all base‑5 permutations of the digits 0–4 with
   a non‑zero first digit that satisfy the adjacency‑product property,
   sort the resulting numbers (the 5‑digit base‑5 strings interpreted as
   decimal digits), and return the nth term. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/prod_base5.h"

/* test whether the digit pattern prod_digits[0..len-1] occurs as a
   contiguous substring of the permutation perm[0..4]. */
static bool contains_substring(const int *perm,
                               const unsigned char *prod_digits,
                               int len)
{
    for (int pos = 0; pos <= 5 - len; ++pos) {
        bool ok = true;
        for (int k = 0; k < len; ++k) {
            if (perm[pos + k] != prod_digits[k]) {
                ok = false;
                break;
            }
        }
        if (ok) return true;
    }
    return false;
}

/* comparison function for qsort on seq_t values */
static int cmp_seq(const void *a, const void *b)
{
    const seq_t *va = (const seq_t *)a;
    const seq_t *vb = (const seq_t *)b;
    if (*va < *vb) return -1;
    if (*va > *vb) return 1;
    return 0;
}

/* static storage for the sequence values */
static seq_t _seq[96];
static size_t _seq_len = 0;
static bool _seq_ready = false;

/* initialise the sequence once */
static void init_seq(void)
{
    if (_seq_ready) return;

    int perm[5];
    bool used[5] = {false};

    for (int d0 = 1; d0 <= 4; ++d0) {
        perm[0] = d0;
        used[d0] = true;
        for (int d1 = 0; d1 <= 4; ++d1) {
            if (used[d1]) continue;
            perm[1] = d1;
            used[d1] = true;
            for (int d2 = 0; d2 <= 4; ++d2) {
                if (used[d2]) continue;
                perm[2] = d2;
                used[d2] = true;
                for (int d3 = 0; d3 <= 4; ++d3) {
                    if (used[d3]) continue;
                    perm[3] = d3;
                    used[d3] = true;
                    for (int d4 = 0; d4 <= 4; ++d4) {
                        if (used[d4]) continue;
                        perm[4] = d4;

                        /* check adjacency–product constraint */
                        bool ok = true;
                        for (int i = 0; i < 4 && ok; ++i) {
                            int prod = perm[i] * perm[i + 1];
                            unsigned char prod_digits[2];
                            int prod_len;
                            prod_base5(prod, prod_digits, &prod_len);
                            if (!contains_substring(perm, prod_digits, prod_len))
                                ok = false;
                        }

                        if (ok) {
                            seq_t val = 0;
                            for (int i = 0; i < 5; ++i)
                                val = val * 10 + perm[i];
                            _seq[_seq_len++] = val;
                        }
                    }
                    used[d3] = false;
                }
                used[d2] = false;
            }
            used[d1] = false;
        }
        used[d0] = false;
    }

    /* sort in increasing decimal value */
    qsort(_seq, _seq_len, sizeof(seq_t), cmp_seq);
    _seq_ready = true;
}

/* public sequence accessor */
seq_t a(idx_t n)
{
    init_seq();

    if (n == 0 || n > _seq_len) {
        return (seq_t)0;
    }
    return _seq[n - 1];
}
