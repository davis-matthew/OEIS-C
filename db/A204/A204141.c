/*
 * A204141  Numbers m such that both m and m/7 are zeroless pandigital numbers
 *          (A050289).
 * https://oeis.org/A204141
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = numbers m such that both m and m/7 are zeroless pandigital numbers.
   This implementation generates all 9‑digit zeroless pandigital numbers (1–9)
   and keeps those that are divisible by 7 with a pandigital quotient.  The list
   is sorted in increasing order and the sequence is 1‑based. */

#include <stdbool.h>
#include <stddef.h>

/* Forward declaration of the function required by the build. */
seq_t a(idx_t n);

/* ---------- Helper functions ---------- */

static bool is_pandigital(seq_t x)
{
    unsigned char used[10] = {0};
    int digits = 0;

    while (x > 0) {
        int d = (int)(x % 10);        /* digit (0–9) */
        if (d == 0 || used[d])        /* zero or repeated digit */
            return false;
        used[d] = 1;
        x /= 10;
        digits++;
    }
    if (digits != 9)                 /* must be 9 digits */
        return false;
    /* all digits 1..9 have appeared */
    for (int i = 1; i <= 9; ++i)
        if (!used[i])
            return false;
    return true;
}

/* ---------- Permutation generator ---------- */

static seq_t seq_values[24];   /* stores the 24 valid terms */
static size_t seq_len = 0;     /* number of terms stored */

static bool used_digit[10];
static int  perm[9];

/* Recursive generation of all permutations of 1..9. */
static void permute(int depth)
{
    if (depth == 9) {
        /* build the number from the current permutation */
        seq_t m = 0;
        for (int i = 0; i < 9; ++i) {
            m = m * (seq_t)10 + (seq_t)perm[i];
        }

        if (m % 7 == 0) {                /* divisible by 7 */
            seq_t q = m / 7;
            if (is_pandigital(q)) {
                seq_values[seq_len++] = m;
            }
        }
        return;
    }

    for (int d = 1; d <= 9; ++d) {
        if (!used_digit[d]) {
            used_digit[d] = true;
            perm[depth] = d;
            permute(depth + 1);
            used_digit[d] = false;
        }
    }
}

/* Build the sequence once, lazily. */
static bool seq_initialized = false;

static void build_sequence(void)
{
    for (int i = 0; i < 10; ++i)
        used_digit[i] = false;
    permute(0);
}

seq_t a(idx_t n)
{
    if (!seq_initialized) {
        build_sequence();
        seq_initialized = true;
    }

    if (n == 0)                 /* offset 1; undefined for n==0 */
        return 0;

    idx_t index = n - 1;
    if (index < seq_len)
        return seq_values[index];
    else
        return 0;               /* no term defined beyond the 24th */
}
