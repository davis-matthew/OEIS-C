/*
 * A105594  Triangle read by rows: abs(A103447)*A047999 mod 2.
 * https://oeis.org/A105594
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 9371, u16 n <= 9371, u32 n <= 9371, u64 n <= 9371,
 *            u128 n <= 9371, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A105594  -  Triangle read by rows: abs(A103447)*A047999 mod 2
 *
 *  For a row r and column c (0‑based) the triangle entry is
 *
 *      T(r,c) = mod( ∑_{j=0}^{r} abs( μ( C(r,j) ) ) * ( C(j,c) mod 2 ) , 2 )
 *
 *  A(n) is the flattened list of the triangle read row by row.
 *  The value T(r,c) is 1 exactly when the number of square‑free binomials
 *  C(r,j) with (c & j) == c is odd.  The implementation below
 *  follows this definition directly.
 */

#include <stdint.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/*  Prime generation (once) ------------------------------------------- */
#define MAX_PRIME 2000          /* sufficient for all rows r ≤ 140        */
#define MAX_PRIMES  400         /* buffer for safety                        */

static bool primes_inited = false;
static int  primes[MAX_PRIMES];
static int  prime_cnt = 0;

static void init_primes(void)
{
    bool is_com[ MAX_PRIME + 1 ] = { false };
    int p, idx = 0;
    for (p = 2; p <= MAX_PRIME; ++p)
    {
        if (!is_com[p])
        {
            primes[idx++] = p;
            if (p * p <= MAX_PRIME)
            {
                int q;
                for (q = p * p; q <= MAX_PRIME; q += p)
                    is_com[q] = true;
            }
        }
    }
    prime_cnt = idx;
    primes_inited = true;
}

/* ------------------------------------------------------------------ */
/*  Legendre symbol: exponent of prime p in n!                         */
static inline unsigned int legendre_exp(unsigned int n, unsigned int p)
{
    unsigned int e = 0;
    while (n)
    {
        n /= p;
        e += n;
    }
    return e;
}

/* ------------------------------------------------------------------ */
/*  Is C(n,k) square‑free?                                              */
static bool binom_is_squarefree(unsigned int n, unsigned int k)
{
    if (k > n) return true;          /* C(n,k)=0, considered square‑free   */
    if (k == 0 || k == n) return true;/* C(n,0)=C(n,n)=1                     */

    for (int i = 0; i < prime_cnt && (unsigned int)primes[i] <= n; ++i)
    {
        unsigned int p = primes[i];
        unsigned int e = legendre_exp(n, p);
        if (k) e -= legendre_exp(k, p);
        if (n - k) e -= legendre_exp(n - k, p);
        if (e >= 2) return false;    /* squared prime factor found          */
    }
    return true;
}

/* ------------------------------------------------------------------ */
/*  Row index of flattened triangle position (0‑based)                 */
static inline idx_t row_of_index(idx_t n)
{
    idx_t lo = 0, hi = (idx_t)MAX_PRIME;
    while (lo < hi)
    {
        idx_t mid = (lo + hi + 1) >> 1;
        __int128 t = (__int128)mid * (mid + 1) / 2;
        if (t <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* ------------------------------------------------------------------ */
/*  Main function ==================================================== */
seq_t a(idx_t n)
{
    if (!primes_inited) init_primes();

    idx_t r = row_of_index(n);
    idx_t c = n - (r * (r + 1)) / 2;

    unsigned char parity = 0;          /* 0/1 parity of the sum */

    for (idx_t j = 0; j <= r; ++j)
    {
        if (binom_is_squarefree((unsigned int)r, (unsigned int)j) &&
            ((c & j) == c))
        {
            parity ^= 1;
        }
    }

    return (seq_t)parity;
}
