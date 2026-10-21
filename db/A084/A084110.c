/*
 * A084110  Let L(n) = ordered list of divisors of n = {d_1=1, d_2, ...,
 *          d_k=n}; set e_1=1, e_i = e_{i-1}/d_i if that is an integer
 *          otherwise e_i = e_{i-1}*d_i; then a(n) = e_k.
 * https://oeis.org/A084110
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 125, u32 n <= 2079, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements A084110:  a(n)=r(n,τ(n))
   where r(n,1)=1 and for j>1,
   r(n,j)= r(n,j‑1)/d_n,j if d_n,j | r(n,j‑1) else r(n,j‑1)*d_n,j
   The function enumerates all divisors of n, sorts them increasingly,
   then applies the rule above.  */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* comparison function for qsort on uint64_t values */
static int cmp_uint64(const void *a, const void *b)
{
    const uint64_t va = *(const uint64_t *)a;
    const uint64_t vb = *(const uint64_t *)b;
    return (va < vb) ? -1 : (va > vb) ? 1 : 0;
}

/* trial‑division factorisation of n, storing prime factors in pr[]
   and their exponents in exp[].  The number of distinct primes is
   written to *cnt.  The routine works for n up to 64 bits. */
static void factor_uint64(uint64_t n, uint64_t *pr, uint64_t *exp, size_t *cnt)
{
    size_t c = 0;
    if (n < 2) { *cnt = 0; return; }

    uint64_t p, e;
    /* handle factor 2 */
    if ((n & 1ULL) == 0) {
        p = 2;
        e = 0;
        while ((n & 1ULL) == 0) { n >>= 1; ++e; }
        pr[c]   = p;
        exp[c]  = e;
        ++c;
    }
    for (p = 3; p <= n / p; p += 2) {
        if (n % p == 0) {
            e = 0;
            while (n % p == 0) { n /= p; ++e; }
            pr[c]   = p;
            exp[c]  = e;
            ++c;
        }
    }
    if (n > 1) {          /* remaining prime factor */
        pr[c]   = n;
        exp[c]  = 1;
        ++c;
    }
    *cnt = c;
}

/* recursive generation of all divisors from the prime factorisation.
   The root call uses cur=1 and idx=0.  The divisors are stored
   sequentially in the array divs, and the total count is returned
   in *dcnt.  Up to 2048 divisors are allowed; this suffices for
   n up to 10 000. */
static void gen_divs_rec(const uint64_t *pr, const uint64_t *exp, size_t cnt,
                         uint64_t cur, size_t idx, uint64_t *divs, size_t *dcnt)
{
    if (idx == cnt) {
        divs[(*dcnt)++] = cur;
        return;
    }
    uint64_t p   = pr[idx];
    uint64_t e   = exp[idx];
    uint64_t c   = cur;
    for (uint64_t i = 0; i <= e; ++i) {
        gen_divs_rec(pr, exp, cnt, c, idx + 1, divs, dcnt);
        c *= p;
    }
}

seq_t a(idx_t n)
{
    uint64_t N = (uint64_t)n;
    if (N == 0) return (seq_t)0;      /* undefined, but safe */

    /* Factorise N */
    uint64_t primes[32];
    uint64_t exps[32];
    size_t pcnt;
    factor_uint64(N, primes, exps, &pcnt);

    /* Generate all divisors */
    uint64_t divs[2048];
    size_t dcnt = 0;
    gen_divs_rec(primes, exps, pcnt, 1ULL, 0, divs, &dcnt);

    /* Sort divisors increasingly */
    qsort(divs, dcnt, sizeof(uint64_t), cmp_uint64);

    /* Apply the definition of r(n,j) over the ordered divisors */
    seq_t e = (seq_t)1;
    for (size_t i = 0; i < dcnt; ++i) {
        seq_t d = (seq_t)divs[i];
        if (e % d == 0) {
            e /= d;
        } else {
            e *= d;
        }
    }
    return e;
}
