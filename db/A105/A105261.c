/*
 * A105261  Values of n such that phi(n)=c(n)^2, where phi is the Euler
 *          totient function and c(n) is the product of the distinct prime
 *          factors of n (c(1)=1).
 * https://oeis.org/A105261
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, nonn, full
 * Formula:   a(n) is the nth integer n such that phi(n) = rad(n)^2
 * Range:     u8 none, u16 none, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Validated: full, pass, 6 terms through n = 6 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Helper to compute φ and rad up to MAX_N */
static void build_phi_rad(idx_t max_n, seq_t *phi, seq_t *rad)
{
    idx_t i;
    for (i = 0; i <= max_n; ++i) {
        phi[i] = (seq_t)i;
        rad[i] = (seq_t)1;
    }
    for (i = 2; i <= max_n; ++i) {
        /* test for primality */
        if (phi[i] == (seq_t)i) {
            seq_t p = (seq_t)i;
            for (idx_t j = i; j <= max_n; j += i) {
                phi[j] -= phi[j] / p;
                rad[j] *= p;
            }
        }
    }
}

/* Find all n <= MAX_N with φ(n) = rad(n)^2 */
static void find_solutions(idx_t max_n, idx_t *count, seq_t **out)
{
    seq_t *phi = mem_new(seq_t, (max_n + 1));
    seq_t *rad = mem_new(seq_t, (max_n + 1));
    if (!phi || !rad) {
        /* allocation failed, leave empty */
        *count = 0;
        *out = NULL;
        return;
    }
    build_phi_rad(max_n, phi, rad);

    idx_t cap = 8;
    idx_t sz = 0;
    seq_t *arr = mem_new(seq_t, cap);
    if (!arr) {
        mem_free(phi);
        mem_free(rad);
        *count = 0;
        *out = NULL;
        return;
    }

    for (idx_t n = 1; n <= max_n; ++n) {
        seq_t r = rad[n];
        if (phi[n] == r * r) {
            if (sz == cap) {
                cap <<= 1;
                seq_t *tmp = mem_resize(seq_t, arr, cap);
                if (!tmp) {
                    /* on failure keep old array */
                    break;
                }
                arr = tmp;
            }
            arr[sz++] = (seq_t)n;
        }
    }

    mem_free(phi);
    mem_free(rad);
    *count = sz;
    *out = arr;
}

/* Public function */
seq_t a(idx_t n)
{
    /* static data for memoisation */
    static bool init = false;
    static idx_t count = 0;
    static seq_t *values = NULL;

    if (!init) {
        const idx_t MAX_N = 200000;  /* larger than largest known term 41154 */
        find_solutions(MAX_N, &count, &values);
        init = true;
    }
    if (n == 0 || n > count) {
        /* Undefined for n beyond known terms: return 0 */
        return (seq_t)0;
    }
    return values[n - 1];
}
