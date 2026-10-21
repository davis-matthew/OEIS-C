/*
 * A070738  Nonprimes n such that n^2 reduced modulo phi(n) = phi(n)^2 reduced
 *          modulo n.
 * https://oeis.org/A070738
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 342, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Property: a(n) = the n‑th non‑prime integer n such that  
   n² mod φ(n) == φ(n)² mod n.  (offset 1) */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Local helpers – all use the supplied integer types. */
static bool is_prime(idx_t x)
{
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    for (idx_t p = 3; p * p <= x; p += 2)
        if (x % p == 0) return false;
    return true;
}

static seq_t phi_of(idx_t n)
{
    seq_t result = n;
    idx_t m = n;
    for (idx_t p = 2; p * p <= m; ++p)
    {
        if (m % p == 0)
        {
            while (m % p == 0) m /= p;
            result -= result / p;
        }
    }
    if (m > 1) result -= result / m;
    return result;
}

static inline seq_t mulmod(seq_t a, seq_t b, seq_t mod)
{
#if defined(__SIZEOF_INT128__)
    unsigned __int128 prod = (unsigned __int128)a * (unsigned __int128)b;
    return (seq_t)(prod % (unsigned __int128)mod);
#else
    return (seq_t)(((unsigned long long)a * (unsigned long long)b) % (unsigned long long)mod);
#endif
}

/* Core sequence routine. */
seq_t a(idx_t n)
{
    /* Static cache of discovered terms */
    static seq_t *terms = NULL;
    static size_t   size  = 0;
    static size_t  cap   = 0;
    static idx_t   next  = 1;      /* next integer to test */
    static idx_t   count = 0;      /* how many terms we have */

    /* Initialise the first allocation */
    if (cap == 0)
    {
        cap   = 128;
        terms = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!terms) return 0;  /* allocation failed – unlikely */
    }

    /* Generate new terms until we have at least n of them */
    while (count < n)
    {
        /* Skip primes */
        while (is_prime(next)) ++next;

        seq_t val      = next;
        seq_t phi_val  = phi_of(next);

        seq_t lhs = mulmod(val, val, phi_val);     /* n² mod φ(n)   */
        seq_t rhs = mulmod(phi_val, phi_val, val); /* φ(n)² mod n   */

        if (lhs == rhs)   /* term satisfies the condition */
        {
            if (count == cap)     /* grow cache */
            {
                size_t new_cap = cap * 2;
                seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                if (!tmp)        /* very unlikely */
                    return val;
                terms = tmp;
                cap   = new_cap;
            }
            terms[count++] = val;
        }
        ++next;
    }

    return terms[n - 1];
}
