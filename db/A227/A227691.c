/*
 * A227691  Winning positions in the Subtract-a-Prime game.
 * https://oeis.org/A227691
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   computes the n-th winning position in the subtract‑a‑prime game
 *            by dynamic programming
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0; /* Undefined for n = 0, harmless */

    /* ----- static state -------------------------------------------- */
    static unsigned char *win          = NULL; /* 0 = lose, 1 = win */
    static size_t win_cap              = 0;
    static unsigned int *primes        = NULL; /* primes found so far */
    static size_t primes_cap           = 0, primes_cnt = 0;
    static seq_t *winners              = NULL; /* list of win positions */
    static size_t wcnt                 = 0, wcap = 0;
    static idx_t computed              = 0;    /* largest number processed */

    /* ----- initialise win array ----------------------------------- */
    if (win_cap == 0) {
        win_cap = 128;
        win = (unsigned char *)malloc(win_cap);
        for (size_t i = 0; i < win_cap; ++i) win[i] = 0; /* lose */
        win[0] = 0;
    }

    /* ----- initialise primes array -------------------------------- */
    if (primes_cap == 0) {
        primes_cap = 64;
        primes = (unsigned int *)malloc(primes_cap * sizeof(unsigned int));
    }

    /* ----- initialise winners list -------------------------------- */
    if (wcap == 0) {
        wcap = 64;
        winners = (seq_t *)malloc(wcap * sizeof(seq_t));
    }

    /* ----- compute until we have at least n winning positions ----- */
    while (wcnt < n) {
        ++computed;  /* next board number */

        /* grow win array if needed */
        if (computed >= (idx_t)win_cap) {
            size_t new_cap = win_cap * 2;
            win = (unsigned char *)realloc(win, new_cap);
            for (size_t i = win_cap; i < new_cap; ++i) win[i] = 0;
            win_cap = new_cap;
        }

        /* test whether 'computed' is prime using known primes */
        bool is_prime = true;
        if (computed < 2) {
            is_prime = false;
        } else {
            for (size_t i = 0; i < primes_cnt; ++i) {
                unsigned int p = primes[i];
                unsigned long long sq = (unsigned long long)p * p;
                if (sq > computed) break;
                if (computed % p == 0) { is_prime = false; break; }
            }
        }

        /* store the prime if found */
        if (is_prime) {
            if (primes_cnt >= primes_cap) {
                size_t new_cap = primes_cap * 2;
                primes = (unsigned int *)realloc(primes, new_cap * sizeof(unsigned int));
                primes_cap = new_cap;
            }
            primes[primes_cnt++] = (unsigned int)computed;
        }

        /* determine win status for 'computed' */
        bool win_this = false;
        for (size_t i = 0; i < primes_cnt; ++i) {
            unsigned int p = primes[i];
            if (p > computed) break;
            if (!win[(size_t)(computed - p)]) { win_this = true; break; }
        }

        win[(size_t)computed] = win_this ? 1 : 0;

        /* record as a winning position if appropriate */
        if (win_this) {
            if (wcnt >= wcap) {
                size_t new_cap = wcap * 2;
                winners = (seq_t *)realloc(winners, new_cap * sizeof(seq_t));
                wcap = new_cap;
            }
            winners[wcnt++] = (seq_t)computed;
        }
    }

    return winners[n - 1];
}
