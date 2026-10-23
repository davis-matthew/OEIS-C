/*
 * A034167  Product of primes == 1 (mod 7) and cubes of primes == 2 or 4 (mod
 *          7).
 * https://oeis.org/A034167
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A034167 – Product of primes ≡ 1 (mod 7) and cubes of primes ≡ 2 or 4 (mod 7).
 * The sequence consists of all positive integers of the form
 *
 *   ∏ p_i × ∏ q_j³
 *
 * where every p_i is a prime with p_i ≡ 1 (mod 7) and every q_j is a prime
 * with q_j ≡ 2 or 4 (mod 7).  The numbers are listed in increasing order.
 *
 * Algorithm: a Hamming‑type multi‑pointer method.  We generate the sequence
 * on demand, keeping a sorted list of candidates base_i * seq[idx_i] for
 * each allowed base.  The algorithm runs in O(n k) time where k is the
 * number of allowed bases (≈ π(limit)), and uses O(n k) space for the
 * generated sequence.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"

/* ------------------------------------------------------------- */
/* Main function required by the build system                       */
/* ------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Static state: generated sequence and generation helpers  */
    static seq_t *seq       = NULL;   /* array of produced values  */
    static size_t seq_len   = 0;       /* number of values stored  */
    static size_t seq_cap   = 0;       /* allocated capacity       */
    /* Bases (p or p³) and indices into seq for each base        */
    static seq_t *bases     = NULL;
    static size_t *idx      = NULL;
    static size_t nbases    = 0;       /* number of bases */

    /* First call: initialise everything.                          */
    if (!seq) {
        /* --------------------------------------------------------- */
        /* 1. Build a list of primes large enough to generate >10000
           terms.  200 000 is plenty for 64‑bit arithmetic and
           keeps memory usage modest.                          */
        /* --------------------------------------------------------- */
        const idx_t PRIME_LIMIT = 200000ULL;
        uint64_t *primes = NULL;
        size_t primes_cnt = 0;
        build_primes_upto(PRIME_LIMIT, &primes, &primes_cnt);

        /* Allocate workspace for bases and indices.  The number
           of allowed bases is at most primes_cnt. */
        bases = (seq_t *)malloc(primes_cnt * sizeof(seq_t));
        idx   = (size_t *)calloc(primes_cnt, sizeof(size_t));
        nbases = 0;

        for (size_t i = 0; i < primes_cnt; ++i) {
            uint64_t p = primes[i];
            if (p % 7 == 1) {
                /* Prime itself is a base. */
                bases[nbases++] = (seq_t)p;
            } else if (p % 7 == 2 || p % 7 == 4) {
                /* Cube of the prime is a base.  Compute the cube safely. */
                if (sizeof(seq_t) <= 8) {
                    unsigned __int128 cube = (unsigned __int128)p * p * p;
                    /* For the 64‑bit build we ignore cubes that would
                       overflow the type. */
                    if (cube <= (unsigned __int128)((seq_t)~(seq_t)0))
                        bases[nbases++] = (seq_t)cube;
                } else {
                    /* Arithmetics with arbitrary‑precision type. */
                    bases[nbases++] = (seq_t)p * p * p;
                }
            }
        }
        free(primes);

        /* --------------------------------------------------------- */
        /* 2. Initialise sequence with the first term 1.           */
        /* --------------------------------------------------------- */
        seq_cap = 16;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        seq[0] = (seq_t)1;
        seq_len = 1;
    }

    /* ------------------------------------------------------------- */
    /* 3. Generate values until we have at least n terms.           */
    /* ------------------------------------------------------------- */
    if (seq_len < n) {
        /* Grow the sequence array if needed. */
        if (seq_cap < (size_t)n) {
            size_t new_cap = seq_cap ? (seq_cap << 1) : 16;
            while (new_cap < (size_t)n)
                new_cap <<= 1;
            seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
            seq_cap = new_cap;
        }

        /* Main generation loop. */
        while (seq_len < n) {
            seq_t next_val = 0;
            bool first = true;

            /* Find the minimal candidate over all bases. */
            for (size_t i = 0; i < nbases; ++i) {
                seq_t cand;
                bool valid = true;
                if (sizeof(seq_t) <= 8) {
                    unsigned __int128 prod =
                        (unsigned __int128)bases[i] * (unsigned __int128)seq[idx[i]];
                    const unsigned __int128 max_val =
                        (unsigned __int128)((seq_t)~(seq_t)0);
                    if (prod > max_val)
                        valid = false;
                    else
                        cand = (seq_t)prod;
                } else {
                    cand = bases[i] * seq[idx[i]];
                }
                if (!valid)
                    continue;
                if (first || cand < next_val) {
                    next_val = cand;
                    first = false;
                }
            }

            /* Append the chosen value. */
            seq[seq_len++] = next_val;

            /* Increment indices for bases that achieved the chosen value. */
            for (size_t i = 0; i < nbases; ++i) {
                seq_t cand;
                bool valid = true;
                if (sizeof(seq_t) <= 8) {
                    unsigned __int128 prod =
                        (unsigned __int128)bases[i] * (unsigned __int128)seq[idx[i]];
                    const unsigned __int128 max_val =
                        (unsigned __int128)((seq_t)~(seq_t)0);
                    if (prod > max_val)
                        valid = false;
                    else
                        cand = (seq_t)prod;
                } else {
                    cand = bases[i] * seq[idx[i]];
                }
                if (!valid)
                    continue;
                if (cand == next_val)
                    ++idx[i];
            }
        }
    }

    /* ------------------------------------------------------------- */
    /* 4. Return the requested term (1‑based indexing).             */
    /* ------------------------------------------------------------- */
    return seq[n - 1];
}
