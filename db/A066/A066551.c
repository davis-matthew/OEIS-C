/*
 * A066551  Let N =149162536496481100121441691962252562893243614..., the
 *          concatenation of the squares. a(n) is the n-digit number formed
 *          from the digits of N starting from the {n(n-1)/2 +1}th digit. Omit
 *          any leading zeros.
 * https://oeis.org/A066551
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n‑digit number formed from the decimal digits of the
 *            infinite string N = 1² 2² 3² … concatenated, starting at the
 *            (n(n−1)/2 + 1)‑th digit (1‑based)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 10, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Cache of already generated digits of N */
static uint8_t *N_digits = NULL;   /* array of decimal digits of N */
static size_t   N_len   = 0;       /* number of digits stored      */
static size_t   N_cap   = 0;       /* allocated capacity           */
static uint64_t next_i  = 1;       /* next integer to square       */

/* Ensure the cache contains at least `needed` digits. */
static void ensure_digits(size_t needed)
{
    if (N_len >= needed)
        return;

    if (N_cap == 0)
        N_cap = 16;
    while (N_cap < needed)
        N_cap <<= 1;

    /* realloc returns void* in C++ – cast explicitly. */
    uint8_t *tmp = (uint8_t *)realloc(N_digits, N_cap);
    if (!tmp)
        abort();
    N_digits = tmp;

    /* Generate subsequent squares until we have enough digits. */
    while (N_len < needed) {
        /* unsigned __int128 is available on GNU/Clang; used for squares
           up to very large numbers. */
        unsigned __int128 sq = (unsigned __int128)next_i * next_i;
        ++next_i;

        /* Convert sq to decimal digits (reversed). */
        char buf[50];          /* 2^128 < 10^39, 50 is plenty. */
        int pos = 0;
        do {
            buf[pos++] = (char)(sq % 10);
            sq /= 10;
        } while (sq);

        /* Append digits in correct order. */
        for (int k = pos - 1; k >= 0; --k)
            N_digits[N_len++] = (uint8_t)buf[k];
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Undefined for n = 0 – return 0 safely. */
    if (n == 0)
        return 0;

    /* 0‑based start index of the substring in N. */
    idx_t start_idx = ((idx_t)n * ((idx_t)n - 1)) / 2;

    /* We need digits up to (start_idx + n - 1). */
    size_t needed_len = (size_t)start_idx + (size_t)n;
    ensure_digits(needed_len);

    /* Build the number from consecutive digits. */
    seq_t result = 0;
    size_t pos = (size_t)start_idx;
    for (size_t k = 0; k < (size_t)n; ++k) {
        result = result * (seq_t)10 + (seq_t)N_digits[pos + k];
    }
    return result;
}
