/*
 * A103611  Smallest prime p with at least two non-overlapping occurrences of
 *          n in decimal representation of p.
 * https://oeis.org/A103611
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = smallest prime p that contains the decimal representation
 *            of n at least twice in p without overlap
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Validated: standard, pass, 52 terms through n = 94 (2026-11-01)
 * Helpers:   is_prime_trial, num_digits0_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_trial.h"
#include "../utils/num_digits0_seq.h"

/* Convert a seq_t value to a decimal string.  The buffer must be
   large enough to hold the result.  */
static void seq_to_dec_string(seq_t val, char *buf, size_t buflen)
{
    seq_t v = val;
    if (v == 0) {
        if (buflen >= 2) {
            buf[0] = '0';
            buf[1] = '\0';
        }
        return;
    }
    size_t pos = 0;
    /* Extract digits in reverse order. */
    while (v != 0 && pos + 1 < buflen) {
        seq_t d = v % 10;
        v = v / 10;
        buf[pos++] = (char)('0' + (int)d);
    }
    /* Null‑terminate and reverse the string. */
    if (pos >= buflen) pos = buflen - 1;
    buf[pos] = '\0';
    for (size_t i = 0; i < pos / 2; ++i) {
        char tmp = buf[i];
        buf[i] = buf[pos - 1 - i];
        buf[pos - 1 - i] = tmp;
    }
}

/*--- dynamic array helpers for primes --------------------------------------*/
static void ensure_primes_cap(size_t *cap, uint64_t **arr, size_t min_cap) {
    if (*cap < min_cap) {
        size_t new_cap = (*cap) ? (*cap) << 1 : 16;
        if (new_cap < min_cap) new_cap = min_cap;
        uint64_t *tmp = (uint64_t *)realloc(*arr, new_cap * sizeof(uint64_t));
        if (!tmp) abort();
        *arr = tmp;
        *cap = new_cap;
    }
}

static seq_t get_prime(uint64_t idx) {
    static uint64_t *prime_vec = NULL;
    static size_t prime_cap = 0;
    static size_t prime_sz  = 0;
    static uint64_t candidate = 3;      /* next odd to test */

    if (!prime_vec) { /* first call: initialise */
        ensure_primes_cap(&prime_cap, &prime_vec, 16);
        prime_vec[0] = 2;
        prime_sz = 1;
    }

    while (prime_sz < idx) {
        /* find next prime */
        while (!is_prime_trial(candidate, prime_vec, prime_sz))
            candidate += 2;

        /* add found prime */
        if (prime_sz == prime_cap)
            ensure_primes_cap(&prime_cap, &prime_vec, prime_cap << 1);

        prime_vec[prime_sz++] = candidate;
        candidate += 2;
    }
    return (seq_t)prime_vec[idx - 1];
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* undefined offset, safe return */

    /* Build the decimal pattern string of n. */
    char pat[32];
    seq_to_dec_string((seq_t)n, pat, sizeof(pat));
    const int pat_len = (int)strlen(pat);

    /* Iterate over primes in order (2, 3, 5, 7, …). */
    idx_t prime_idx = 1;                      /* get_prime(1) == 2 */
    for (;; ++prime_idx)
    {
        seq_t cp = get_prime(prime_idx);      /* candidate prime */

        /* Convert candidate to decimal string. */
        idx_t cp_len = num_digits0_seq(cp);
        char *buf = (char*)malloc(cp_len + 1);
        if (!buf) return 0;                  /* allocation failure -> abort */
        seq_to_dec_string(cp, buf, cp_len + 1);

        /* Search for two non‑overlapping occurrences of pat. */
        const char *p1 = strstr(buf, pat);
        if (p1)
        {
            const char *p2 = strstr(p1 + pat_len, pat);
            if (p2)
            {
                free(buf);
                return cp;                   /* found suitable prime */
            }
        }
        free(buf);
    }
}
