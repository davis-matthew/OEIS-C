/*
 * A015820  Numbers k such that phi(k + 4) | sigma(k).
 * https://oeis.org/A015820
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Numbers k such that φ(k + 4) | σ(k)
 * Range:     u8 n <= 27, u16 n <= 764, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* a(n) – nth such integer (a(1)=5) */
seq_t a(idx_t n) {
    static seq_t *terms = NULL;   /* list of found k values      */
    static size_t cap = 0;        /* capacity of 'terms' array  */
    static size_t count = 0;      /* number of terms stored     */
    static idx_t last_k = 0;      /* last k that was examined   */

    if (n == 0) return 0;          /* undefined but safe value   */

    /* Generate terms until we have at least n of them */
    while (count < n) {
        idx_t k = last_k + 1;                /* next candidate            */

        /* ---- compute σ(k) -------------------------------------- */
        uint64_t t = k;
        uint64_t sigma = 1;
        for (uint64_t p = 2; p * p <= t; ++p) {
            if (t % p == 0) {
                int exp = 0;
                while (t % p == 0) {
                    t /= p;
                    exp++;
                }
                /* contribution to σ(k) */
                uint64_t pow = 1, term = 1;
                for (int i = 0; i < exp; ++i) {
                    pow *= p;
                    term += pow;
                }
                sigma *= term;
            }
        }
        if (t > 1) {                       /* remaining prime factor   */
            sigma *= (uint64_t)(1 + t);
        }

        /* ---- compute φ(k + 4) ---------------------------------- */
        uint64_t m = (uint64_t)k + 4;
        uint64_t phi = m;
        t = m;
        for (uint64_t p = 2; p * p <= t; ++p) {
            if (t % p == 0) {
                while (t % p == 0) t /= p;
                phi -= phi / p;
            }
        }
        if (t > 1) phi -= phi / t;          /* last prime factor        */

        /* ---- check divisibility -------------------------------- */
        if ((seq_t)(sigma % phi) == (seq_t)0) {
            /* store the term */
            if (count >= cap) {
                size_t new_cap = (cap == 0 ? 16 : cap * 2);
                seq_t *new_terms = (seq_t *)realloc(terms,
                                        new_cap * sizeof(seq_t));
                if (!new_terms) exit(1);
                terms = new_terms;
                cap = new_cap;
            }
            terms[count++] = (seq_t)k;
        }

        last_k = k;   /* advance to next candidate         */
    }

    return terms[n - 1];   /* zero‑based indexing in the array   */
}
