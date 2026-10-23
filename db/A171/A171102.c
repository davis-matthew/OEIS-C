/*
 * A171102  Pandigital numbers: numbers containing the digits 0-9. Version 2:
 *          each digit appears at least once.
 * https://oeis.org/A171102
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 1000, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A171102 : Pandigital numbers (each digit 0‑9 at least once)
    First 9*9! = 3 265 920 terms are permutations of 0‑9
    with a non‑zero leading digit, ordered increasingly.
    After that terms are found by scanning upward.  */

#include <stddef.h>
#include <stdbool.h>

/* factorials 0! … 10!  (used only for the first 10‑digit block) */
static const unsigned long long fact[11] = {
    1ULL, 1ULL, 2ULL, 6ULL, 24ULL, 120ULL, 720ULL,
    5040ULL, 40320ULL, 362880ULL, 3628800ULL
};

/* orange digit list 0‑9 (array index = digit value)  */
static const unsigned char digits10[10] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9
};

/* Return true iff x contains each decimal digit 0‑9 at least once. */
static bool has_all_digits(seq_t x)
{
    unsigned mask = 0;
    while (x != (seq_t)0) {
        int d = (int)(x % (seq_t)10);
        mask |= 1u << d;
        if (mask == 1023)        /* 0b1111111111  (all ten bits set) */
            return true;
        x /= (seq_t)10;
    }
    return mask == 1023;
}

/* Produce the nth 10‑digit pandigital (n within 1 … 9*9!).  */
static seq_t pandigital_10(idx_t n)
{
    /* n is 1‑based; convert to 0‑based rank in the block. */
    idx_t rank = n - 1ULL;
    /* first digit: all 9 possibilities, each followed by 9! permutations */
    idx_t first_idx = rank / fact[9];
    unsigned first_digit = digits10[first_idx + 1];   /* skip 0 */

    /* remaining rank after fixing first digit */
    idx_t rem_rank = rank % fact[9];

    /* build list of remaining digits (sorted) */
    unsigned rem[9];
    unsigned k = 0;
    for (unsigned d = 0; d <= 9; ++d) {
        if (d != first_digit)
            rem[k++] = d;
    }

    seq_t result = (seq_t)first_digit;
    /* Generate the remaining 9 digits via factorial number system. */
    for (int pos = 1; pos <= 9; ++pos) {
        idx_t f = fact[9 - pos];
        idx_t idx = rem_rank / f;
        rem_rank %= f;

        unsigned digit = rem[idx];
        result = result * (seq_t)10 + (seq_t)digit;

        /* remove chosen digit from remaining list */
        for (unsigned j = idx; j < 8; ++j)
            rem[j] = rem[j + 1];
    }
    return result;
}

/* --------------------------------------------------------------- */
/*  Main function required by the build system */
seq_t a(idx_t n)
{
    if (n == 0)         /* safety: offset unspecified */
        return (seq_t)0;

    /* maximum index covered by the 10‑digit block */
    const idx_t MAX_PERMS = 9ULL * fact[9];  /* 3 265 920 */

    if (n <= MAX_PERMS)          /* 10‑digit pandigital part  */
        return pandigital_10(n);

    /* For n > MAX_PERMS we simply scan upward after the largest
       10‑digit pandigital.  This is fast enough for the required
       N ≤ 10 000, and works for any larger N too (though it may be
       slower). */
    seq_t candidate = (seq_t)9876543210ULL;  /* last 10‑digit pandigital */
    idx_t cnt = MAX_PERMS;

    while (true) {
        ++candidate;
        if (has_all_digits(candidate)) {
            ++cnt;
            if (cnt == n)
                return candidate;
        }
    }
}
