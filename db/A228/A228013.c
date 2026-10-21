/*
 * A228013  The smallest n-digit number whose last k digits are divisible by
 *          k^2 for k = 1..n, otherwise 0 (for n > 1).
 * https://oeis.org/A228013
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the smallest n‑digit number whose last k digits are
 *            divisible by k^2 for k = 1..n, otherwise 0. The computation is
 *            performed by building the number from the least significant
 *            digit to the most significant digit, keeping all partial
 *            suffixes that satisfy the constraints so far. If no suffix...
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 23, u128 n <= 39,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../utils/seq_from_str.h"

static uint64_t pow10_mod(uint64_t exp, uint64_t mod)
{
    uint64_t res = 1;
    for (uint64_t i = 0; i < exp; ++i)
        res = (res * 10ULL) % mod;
    return res;
}

seq_t a(idx_t n)
{
    /* start with all one–digit suffixes */
    size_t states_cnt = 10;
    char **states = (char **)malloc(states_cnt * sizeof(char *));
    for (int d = 0; d <= 9; ++d) {
        states[d] = (char *)malloc(2);
        states[d][0] = '0' + d;
        states[d][1] = '\0';
    }

    /* build up the number digit by digit from the right */
    for (uint64_t k = 1; k < (uint64_t)n; ++k) {
        uint64_t m = (k + 1) * (k + 1);            /* modulus for the new constraint   */
        uint64_t pow10_k_mod = pow10_mod(k, m);     /* 10^k mod m                        */

        size_t new_cap = 0, new_cnt = 0;
        char **new_states = NULL;

        for (size_t i = 0; i < states_cnt; ++i) {
            char *prev = states[i];

            /* remainder of the previous suffix modulo the new modulus */
            uint64_t rem = 0;
            for (char *p = prev; *p; ++p)
                rem = (rem * 10ULL + (uint64_t)(*p - '0')) % m;

            for (int d = 0; d <= 9; ++d) {
                /* the most significant digit of a full number may not be 0 (except for n == 1) */
                if (k + 1 == n && n > 1 && d == 0)
                    continue;

                uint64_t new_rem = ( (uint64_t)d * pow10_k_mod + rem ) % m;
                if (new_rem == 0) {
                    size_t new_len = (size_t)(k + 1);
                    char *s = (char *)malloc(new_len + 1);
                    s[0] = '0' + d;
                    memcpy(s + 1, prev, new_len);
                    s[new_len] = '\0';

                    if (new_cnt == new_cap) {
                        new_cap = new_cap ? 2 * new_cap : 8;
                        new_states = (char **)realloc(new_states, new_cap * sizeof(char *));
                    }
                    new_states[new_cnt++] = s;
                }
            }
        }

        /* free old states */
        for (size_t i = 0; i < states_cnt; ++i) free(states[i]);
        free(states);

        if (new_cnt == 0)          /* no suffix satisfies the constraints */
            return seq_from_str("0");

        states = new_states;
        states_cnt = new_cnt;
    }

    /* among all full-length solutions pick the smallest one */
    size_t best_idx = 0;
    for (size_t i = 1; i < states_cnt; ++i)
        if (strcmp(states[i], states[best_idx]) < 0)
            best_idx = i;

    seq_t result = seq_from_str(states[best_idx]);

    /* free all dynamically allocated strings */
    for (size_t i = 0; i < states_cnt; ++i)
        free(states[i]);
    free(states);

    return result;
}
