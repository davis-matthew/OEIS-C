/*
 * A141575  A gap prime-type triangular sequence of coefficients:
 *          gap(n)=Prime[n+1]-Prime[n]; t(n,m)=If[n == m == 0, 1, If[m == 0,
 *          ((Prime[n] + gap[n])^ n + (Prime[n] - gap[n])^n)/2, ((Prime[n] +
 *          gap[n]*Sqrt[Prime[m]])^n + (Prime[n] -
 *          gap[n]*Sqrt[Prime[m]])^n)/2]].
 * https://oeis.org/A141575
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, uned
 * Formula:   a(n) = t(r,m) where n = 1-based index in a flattened triangular
 *            table. For r = 0,1,2,... and m = 0,1,...,r, t(r,m) = X_r with
 *            X_0 = 1, X_1 = Prime[r], and for r≥2: X_{k+1} = 2*Prime[r]*X_k -
 *            (Prime[r]^2 - gap^2*D)*X_{k-1}, where gap = Prime[r+1]-Prime[r]
 *            and D = 1 if m==0, otherwise D = Prime[m]...
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Validated: partial, pass, 32 terms through n = 32 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* Return the k‑th prime (k ≥ 1).  On first call the prime list
   is built incrementally using trial division.  The list is
   stored in a static array which is expanded as necessary.   */
static uint32_t get_prime(idx_t k)
{
    static uint32_t *pr = NULL;     /* array of primes */
    static size_t sz   = 0;         /* number of primes stored */
    static size_t cap  = 0;         /* allocated capacity */
    static uint32_t cand = 3;       /* candidate after 2 */

    /* initialise the prime table on first use */
    if (sz == 0) {
        cap = 8;
        pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
        if (!pr) abort();
        sz = 0;
    }

    /* grow the array until we have at least k primes */
    while (sz < (size_t)k) {
        if (sz == 0) {                /* add the first prime */
            pr[sz++] = 2;
            continue;
        }

        /* find the next prime candidate */
        uint32_t p = cand;
        bool ok = true;
        for (size_t i = 0; i < sz; ++i) {
            uint32_t prime = pr[i];
            if ((uint64_t)prime * prime > (uint64_t)p) break;
            if (p % prime == 0) { ok = false; break; }
        }

        if (!ok) {                    /* not prime – keep searching */
            cand += 2;                /* only odd numbers remain */
            continue;
        }

        /* prime found – store it, grow table if needed */
        if (sz == cap) {
            cap <<= 1;
            pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
            if (!pr) abort();
        }
        pr[sz++] = p;
        cand = p + 2;
    }

    return pr[(size_t)k - 1];
}

seq_t a(idx_t n)
{
    /* Convert to 0‑based index */
    idx_t idx = n - 1;

    /* Find the triangular row r such that cumulative terms before r
       are <= idx < cumulative terms up to r. */
    idx_t cumulative = 0;
    idx_t r = 0;
    while (cumulative + (r + 1) <= idx) {
        cumulative += r + 1;
        ++r;
    }
    idx_t m = idx - cumulative;      /* 0‑based position in the row */

    /* Special case: first row r = 0 has only one element t(0,0)=1 */
    if (r == 0) {
        return (seq_t)1;
    }

    /* Prime indices are 1‑based: Prime[1]=2, Prime[2]=3, ... */
    seq_t prime_r   = (seq_t)get_prime(r);     /* Prime[r]   */
    seq_t prime_r_p = (seq_t)get_prime(r + 1); /* Prime[r+1] */

    seq_t gap = prime_r_p - prime_r;           /* gap(r) = Prime[r+1] - Prime[r] */

    /* Determine D: 1 for m==0, otherwise the m‑th prime (1‑based) */
    seq_t D = (m == 0) ? (seq_t)1 : (seq_t)get_prime((idx_t)m);

    /* Recurrence for X_r (which equals a(n)) */
    seq_t X_prev = (seq_t)1;   /* X_0 */
    if (r == 0) return X_prev;/* just in case */

    seq_t X_curr = prime_r;    /* X_1 = Prime[r] */
    if (r == 1) return X_curr;

    /* Coefficient: a^2 - gap^2 * D */
    seq_t coeff = prime_r * prime_r - gap * gap * D;

    /* Iterate the recurrence up to r */
    for (idx_t k = 1; k < r; ++k) {
        seq_t term1 = 2 * prime_r * X_curr;   /* 2*a*X_k */
        seq_t term2 = coeff * X_prev;         /* coeff*X_{k-1} */
        seq_t X_next = term1 - term2;
        X_prev = X_curr;
        X_curr = X_next;
    }

    return X_curr;
}
