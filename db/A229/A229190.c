/*
 * A229190  Beginning position of n in the decimal expansion of the
 *          Copeland-Erdos constant.
 * https://oeis.org/A229190
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   a(n) = starting position (1‑based) of the decimal representation
 *            of n in the Copeland–Erdős constant (the concatenation of all
 *            primes)
 * Range:     u8 n <= 19, u16 n <= 999, u32 n <= 999, u64 n <= 999,
 *            u128 n <= 999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

/* Helper: primality test for 64‑bit unsigned integers */
static bool isPrime(idx_t x) {
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    for (idx_t d = 3; d <= x / d; d += 2) {
        if (x % d == 0) return false;
    }
    return true;
}

/* Main function */
seq_t a(idx_t n) {
    /* Convert n to its decimal string and store in pat[0..m-1] */
    char pat[21];
    size_t m = 0;
    if (n == 0) {
        pat[0] = '0';
        m = 1;
    } else {
        char rev[21];
        idx_t t = n;
        while (t > 0) {
            rev[m++] = '0' + (t % 10);
            t /= 10;
        }
        for (size_t i = 0; i < m; ++i) pat[i] = rev[m - 1 - i];
    }

    /* Build KMP failure function for pat */
    size_t fail[21];
    fail[0] = 0;
    for (size_t i = 1; i < m; ++i) {
        size_t j = fail[i - 1];
        while (j > 0 && pat[i] != pat[j]) j = fail[j - 1];
        if (pat[i] == pat[j]) ++j;
        fail[i] = j;
    }

    /* Stream through primes and feed digits into the KMP automaton */
    size_t matched = 0;      /* number of pattern chars matched so far */
    idx_t pos = 0;           /* current position in the Copeland–Erdős decimal string */

    idx_t num = 2;
    for (;;) {
        if (isPrime(num)) {
            /* Convert prime to decimal digit array in proper order */
            char buf[21];
            size_t len = 0;
            idx_t x = num;
            while (x > 0) {
                buf[len++] = '0' + (x % 10);
                x /= 10;
            }
            for (size_t k = 0; k < len / 2; ++k) {
                char tmp = buf[k];
                buf[k] = buf[len - 1 - k];
                buf[len - 1 - k] = tmp;
            }

            /* Feed the digits */
            for (size_t k = 0; k < len; ++k) {
                char c = buf[k];
                ++pos;                          /* advance position */
                while (matched > 0 && pat[matched] != c) matched = fail[matched - 1];
                if (pat[matched] == c) ++matched;
                if (matched == m) {              /* match found */
                    return (seq_t)(pos - (idx_t)m + 1);
                }
            }
        }

        /* Advance to next candidate prime */
        if (num == 2) num = 3;
        else num += 2;
    }
}
