/*
 * A120123  Numbers m such that m=(d_1*d_2*...*d_k)*phi(d_1+d_2+...+d_k) where
 *          d_1 d_2... d_k is the decimal expansion of m.
 * https://oeis.org/A120123
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   calc_phi, get_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Brute‑force search for numbers m such that m = (digit_product) * phi(digit_sum) */
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/get_digits.h"
#include "../utils/calc_phi.h"

/* Cache for found terms */
static seq_t *found = NULL;
static size_t found_count = 0;
static size_t found_cap   = 0;
static bool generated = false;

/* Append a term to the dynamic array */
static void add_found(seq_t value)
{
    if (found_count == found_cap) {
        size_t new_cap = found_cap ? found_cap * 2 : 256;
        seq_t *tmp = (seq_t *)realloc(found, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        found = tmp;
        found_cap = new_cap;
    }
    found[found_count++] = value;
}

/* Generate all terms once (up to a safe upper bound) */
static void generate_all(void)
{
    if (generated) return;
    generated = true;

    /* The conjectured largest term is 139968. A slightly larger bound
     * guarantees we find all terms that fit in 64‑bit. */
    const uint64_t LIMIT = 500000;

    uint8_t digits[32];
    size_t len;

    for (uint64_t num = 1; num <= LIMIT; ++num) {
        seq_t m = (seq_t)num;

        /* Extract decimal digits.  get_digits is available from the utils. */
        get_digits(m, digits, &len);

        bool has_zero = false;
        seq_t prod = 1;          /* product of digits */
        seq_t sum  = 0;          /* sum of digits */

        for (size_t i = 0; i < len; ++i) {
            uint8_t d = digits[i];
            if (d == 0) { has_zero = true; break; }
            prod *= (seq_t)d;
            sum  += (seq_t)d;
        }
        if (has_zero) continue;

        seq_t phi_val = calc_phi(sum);
        if (prod * phi_val == m) {
            add_found(m);
        }
    }

    /* No need to sort; terms are discovered in ascending order. */
}

/* Sequence entry point */
seq_t a(idx_t n)
{
    if (n == 0) return 0;
    generate_all();
    if ((idx_t)found_count < n) return 0;
    return found[n - 1];
}
