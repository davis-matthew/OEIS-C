/*
 * A097505  Triangle read by rows: T(n,k) = Sum_{j=1..k} Prime(n+j-1).
 * https://oeis.org/A097505
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 3915, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) is the entry of the triangle T(row,col)
 * where row=row(n), col=col(n) and
 * T(row,col) = sum_{j=1}^{col} prime(row+j-1).
 * The triangle is read row by row, offset 1.
 */
#include <stdint.h>
#include <stddef.h>

/* Prime table ----------------------------------------------------------- */
/* We store primes up to the current maximum needed index.  Since the
   sequence requires at most the first ~2n primes for a(n) with row n,
   a table of 200 000 primes is more than enough for n ≤ 10 000. */
static uint32_t primes[200000];
static size_t prime_cnt = 0;   /* number of primes stored so far */
static uint32_t next_cand = 2; /* next integer to test for primality */

/* Ensure that at least need primes are stored in the table.
   Uses simple trial division with already found primes. */
static void ensure_primes(idx_t need)
{
    while ((idx_t)prime_cnt < need)
    {
        if (next_cand == 2)
        {
            primes[prime_cnt++] = 2;
            next_cand = 3;
            continue;
        }

        uint32_t cand = next_cand;
        uint32_t limit = (uint32_t)((uint64_t)cand * cand);
        int is_prime = 1;

        for (size_t i = 0; i < prime_cnt; ++i)
        {
            uint32_t p = primes[i];
            if ((uint64_t)p * p > cand)
                break;
            if (cand % p == 0)
            {
                is_prime = 0;
                break;
            }
        }

        if (is_prime)
        {
            primes[prime_cnt++] = cand;
        }
        next_cand += 2; /* skip even numbers */
    }
}

/* Return the i‑th prime (1‑based index).  The result is converted to seq_t. */
static seq_t get_prime(idx_t i)
{
    ensure_primes(i);
    return (seq_t)primes[i - 1];
}

/* Compute the row and column in the triangle for the flattened
   index m (1‑based).  Returns row and column 1‑based. */
static void row_col(idx_t m, idx_t *row, idx_t *col)
{
    /* Find smallest n such that n*(n+1)/2 >= m */
    idx_t lo = 0, hi = m;
    while (lo < hi)
    {
        idx_t mid = lo + (hi - lo) / 2;
        idx_t tri = mid * (mid + 1) / 2;
        if (tri >= m)
            hi = mid;
        else
            lo = mid + 1;
    }
    *row = lo;
    *col = m - (lo - 1) * lo / 2;
}

/* Sequence generator ----------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0; /* undefined for n=0, but safe */

    idx_t r, k;
    row_col(n, &r, &k);

    seq_t sum = 0;
    idx_t start = r;           /* first prime index in the sum */
    idx_t end   = r + k - 1;   /* last prime index in the sum */

    for (idx_t p = start; p <= end; ++p)
    {
        sum += get_prime(p);
    }
    return sum;
}
