/*
 * A011742  A binary m-sequence: expansion of reciprocal of x^29 + x^2 + 1
 *          (mod 2, shifted by 28 initial 0's).
 * https://oeis.org/A011742
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Let b(n) be the coefficient of x^n in the power‑series *
 *            expansion of (1 + x^2 + x^29)^(-1) over GF(2). It satisfies *
 *            b(0) = 1; b(n) = b(n-2) ⊕ b(n-29) for n > 0. * The OEIS sequence
 *            is a left shift of these coefficients by * 28 zeros: a(n) = 0
 *            for n < 28, and a(n) = b(n-28) for n ≥ 28
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *coeff = NULL;      /* storage for b array          */
static size_t coeff_cap = 0;     /* allocated capacity of coeff  */
static size_t coeff_computed = 0; /* number of entries already computed */

/* Ensure the coeff array can hold at least 'needed' elements. */
static void ensure_capacity(size_t needed)
{
    if (coeff_cap >= needed)
        return;
    size_t new_cap = coeff_cap ? coeff_cap : 1;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(coeff, new_cap * sizeof(seq_t));
    if (!tmp) {
        /* allocation failure – aborting is acceptable for this build. */
        abort();
    }
    coeff = tmp;
    coeff_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n < 28)             /* shifted zeros before the first 1 */
        return 0;

    size_t m = (size_t)(n - 28);   /* index into the b sequence */

    if (coeff_computed <= m) {     /* need to generate more terms */
        ensure_capacity(m + 1);

        if (coeff_computed == 0) { /* first time: set base value b(0)=1 */
            coeff[0] = (seq_t)1;
            coeff_computed = 1;
        }

        for (size_t i = coeff_computed; i <= m; ++i) {
            seq_t low  = (i >= 2)  ? coeff[i - 2]  : (seq_t)0;
            seq_t high = (i >= 29) ? coeff[i - 29] : (seq_t)0;
            coeff[i] = low ^ high;          /* XOR mod 2 */
        }
        coeff_computed = m + 1;
    }

    return coeff[m];
}
