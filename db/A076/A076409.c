/*
 * A076409  Sum of the quadratic residues of prime(n).
 * https://oeis.org/A076409
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Sum of the quadratic residues of prime(n). A076409: a(n) =
 *            sum_{x=1}^{p-1} x^2 mod p, where p = prime(n). For p ≡ 1 mod 4:
 *            a = k*p, k = (p-1)/4. For p ≡ 3 mod 4 (p>3): a = (k - b)*p, k =
 *            (p-3)/4, b = (h(-p)-1)/2, h(-p) = class number of Q(√-p). For p
 *            = 2 or 3: a = 1. The class number is computed...
 * Range:     u8 n <= 11, u16 n <= 97, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 64 terms through n = 847 (2026-11-01)
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

/* Compute class number h(-p) for p an odd prime ≡ 3 (mod 4).
   Enumerate reduced binary quadratic forms of discriminant D = -p. */
static uint64_t class_number_neg_prime(uint32_t p)
{
    int64_t D = -(int64_t)p;        /* discriminant (negative) */
    uint64_t h = 0;

    /* upper bound for a: a^2 <= |D|/3  (since a <= c and b^2 <= |D| + 4ac) */
    int64_t a_max = 0;
    while ((a_max + 1) * (a_max + 1) <= (int64_t)(p / 3)) {
        a_max++;
    }

    for (int64_t a = 1; a <= a_max; ++a) {
        /* b must have same parity as D (odd) and satisfy |b| <= a. */
        int64_t start = -a;
        if ((start & 1) == 0) start++;          /* ensure b is odd */
        for (int64_t b = start; b <= a; b += 2) {
            /* equation: b^2 - D = 4ac  =>  c = (b^2 - D)/(4a) */
            int64_t b2 = b * b;
            int64_t val = b2 - D;               /* val = b^2 - D > 0 */
            int64_t mod = val % (4 * a);
            if (mod != 0) continue;

            int64_t c = val / (4 * a);
            if (a > c) continue;               /* require a <= c */

            /* reducedness condition */
            if ((llabs(b) == a) || (a == c)) {
                if (b < 0) continue;          /* require b >= 0 if |b|==a or a==c */
            }

            /* valid reduced form found */
            h++;
        }
    }

    return h;
}

seq_t a(idx_t n)
{
    uint32_t p = get_prime(n);          /* p is prime(n), 1‑based */
    if (p <= 3)                         /* a(1)=a(2)=1 */
        return (seq_t)1;

    if (p % 4 == 1) {                   /* p = 4k+1 */
        seq_t k = (seq_t)((p - 1) / 4);
        return k * (seq_t)p;
    } else {                            /* p = 4k+3, p > 3 */
        seq_t k = (seq_t)((p - 3) / 4);
        uint64_t h = class_number_neg_prime(p);
        seq_t b = (seq_t)((h - 1) / 2);
        return (k - b) * (seq_t)p;
    }
}
