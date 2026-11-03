/*
 * A037935  Trajectory of 36 under prime factor concatenation procedure.
 * https://oeis.org/A037935
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  fini, full, nonn, tabf, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Validated: full, pass, 9 terms through n = 8 (2026-11-03)
 * Helpers:   mem, num_digits_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Trajectory of 36 under prime factor concatenation procedure.
   a(0) = 36
   For a composite n:
     * output every prime factor of n (with multiplicity)
     * output the number of prime factors if it's equal to one of them
     * then form the next composite by concatenating the prime factors
       and the number of factors (if it was output).  Repeat.
   For a prime n:
     * output n and stop.
   This generates the sequence
     36, 2, 2, 3, 3, 7, 11, 29, 71129, …            */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/num_digits_idx.h"
#include "../utils/mem.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

static inline uint64_t minimal_factor(uint64_t n)
{
    if (n % 2ULL == 0ULL) return 2ULL;
    if (n % 3ULL == 0ULL) return 3ULL;
    for (uint64_t d = 5ULL; d <= n / d; d += 6ULL) {
        if (n % d == 0ULL)        return d;
        if (n % (d + 2ULL) == 0ULL) return d + 2ULL;
    }
    return n;          /* n is prime */
}

static inline void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t newcap = (*cap == 0) ? 8 : (*cap << 1);
        if (newcap < need) newcap = need;
        *arr = mem_resize(seq_t, *arr, newcap);
        *cap = newcap;
    }
}

/* Computes the n-th term of the trajectory of 36. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap  = 0;
    static size_t len  = 0;     /* number of terms stored */
    static seq_t  cur  = 0;     /* current composite being processed */

    /* first call: initialise */
    if (len == 0) {
        grow_seq(&cache, &cap, 8);
        cache[0] = (seq_t)36;
        len = 1;
        cur = (seq_t)36;
    }

    /* generate further terms until we reach at least n+1 terms */
    while (len <= (size_t)n) {
        /* factor cur into primes (using uint64_t because values stay small) */
        uint64_t tmp = (uint64_t)cur;
        seq_t   *primes = NULL;
        size_t   pcap  = 0;
        size_t   psiz  = 0;

        if (tmp == 1) break;   /* should not happen with starting 36 */

        while (tmp > 1) {
            uint64_t p = minimal_factor(tmp);
            grow_seq(&primes, &pcap, psiz + 1);
            primes[psiz++] = (seq_t)p;
            tmp /= p;
        }

        /* check if cur is prime: only one prime factor equal to cur */
        if (psiz == 1 && primes[0] == cur) {
            /* prime: record it and finish */
            grow_seq(&cache, &cap, len + 1);
            cache[len++] = cur;
            mem_free(primes);
            break;
        }

        /* composite: record each prime factor */
        for (size_t i = 0; i < psiz; ++i) {
            grow_seq(&cache, &cap, len + 1);
            cache[len++] = primes[i];
        }

        /* count of prime factors */
        uint64_t count = psiz;
        bool count_in = false;
        for (size_t i = 0; i < psiz; ++i)
            if ((seq_t)count == primes[i]) { count_in = true; break; }

        if (count_in) {
            grow_seq(&cache, &cap, len + 1);
            cache[len++] = (seq_t)count;
        }

        /* compute next composite by concatenation */
        seq_t next = (seq_t)0;
        for (size_t i = 0; i < psiz; ++i) {
            int d = num_digits_idx((idx_t)primes[i]);
            seq_t pow = pow10((idx_t)d);
            next = next * pow + primes[i];
        }
        if (count_in) {
            int d = num_digits_idx((idx_t)count);
            seq_t pow = pow10((idx_t)d);
            next = next * pow + (seq_t)count;
        }

        mem_free(primes);
        cur = next;
    }

    return cache[(size_t)n];
}
