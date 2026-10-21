/*
 * A069809  Numbers k such that gcd(k, phi(k)) = tau(k).
 * https://oeis.org/A069809
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) is the nth integer k such that gcd(k, phi(k)) = τ(k)
 * Range:     u8 n <= 17, u16 n <= 1621, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   calc_phi, divisor_count_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/divisor_count_u64.h"
#include "../utils/calc_phi.h"

static seq_t *seq_arr = NULL;   /* array of found k */
static size_t seq_len = 0;      /* number of elements stored */
static size_t seq_cap = 0;      /* current capacity of seq_arr */
static uint64_t checked_max = 0; /* largest candidate examined */

static void seq_append(seq_t val)
{
    if (seq_len == seq_cap) {
        seq_cap = seq_cap ? seq_cap * 2 : 64;
        seq_t *tmp = (seq_t *)realloc(seq_arr, seq_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        seq_arr = tmp;
    }
    seq_arr[seq_len++] = val;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                    /* undefined for n=0 */

    while (seq_len < (size_t)n) {
        ++checked_max;                       /* next candidate k */

        /* τ(k) for 64‑bit k */
        uint64_t tau = divisor_count_u64(checked_max);

        /* φ(k) and gcd(k, φ(k)) using seq_t operations */
        seq_t phi  = calc_phi((seq_t)checked_max);
        seq_t g_c  = gcd((seq_t)checked_max, phi);

        if (g_c == (seq_t)tau) {
            seq_append((seq_t)checked_max);
        }
    }
    return seq_arr[(size_t)(n - 1)];
}
