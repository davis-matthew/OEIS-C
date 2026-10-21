/*
 * A060457  Number of solutions to y^2 + y = x^3 - x^2 modulo n.
 * https://oeis.org/A060457
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = number of solutions (x,y) modulo n of y² + y = x³ – x²
 *            The implementation uses the multiplicativity of the number of
 *            solutions with respect to coprime moduli: a(n) = ∏ a(p^e) for n
 *            = ∏ p^e Each a(p^e) is computed by counting values of L(y)=y² +
 *            y (mod p^e) and R(x)=x³ – x² (mod p^e) in lin...
 * Range:     u8 n <= 83, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <string.h>

/* ------------------------------------------------------------ */
/*  prime sieve and table of smallest prime factor  (spf)       */
/* ------------------------------------------------------------ */
static int spf[10001];
static int spf_init = 0;

static void init_spf(void)
{
    int i, j;
    if (spf_init) return;
    spf_init = 1;
    for (i = 0; i <= 10000; ++i) spf[i] = 0;
    for (i = 2; i <= 10000; ++i)
    {
        if (spf[i] == 0)
        {
            spf[i] = i;
            if ((int64_t)i * i <= 10000)
                for (j = i * i; j <= 10000; j += i)
                    if (spf[j] == 0) spf[j] = i;
        }
    }
}

/* ------------------------------------------------------------ */
/*  memoisation tables                                       */
/* ------------------------------------------------------------ */
static seq_t cache[10001];            /* a(n)  for n≤10000, 0 if not yet computed */
static seq_t primepow_cnt[10001];     /* a(p^e)  for each prime power ≤10000 */

/* one-time buffer used to count L(y) values */
static seq_t tmp_counts[10001];

/* ------------------------------------------------------------ */
/*  compute a(p^e)  (prime power modulus)                      */
/* ------------------------------------------------------------ */
static seq_t prime_power_count(uint32_t m)
{
    seq_t cnt = 0;
    uint32_t i;
    /* zero the buffer up to m */
    memset(tmp_counts, 0, m * sizeof(seq_t));

    /* count occurrences of L(y) = y*y + y  (mod m) */
    for (i = 0; i < m; ++i)
    {
        uint32_t yy = (uint64_t)i * i + i;
        uint32_t val = yy % m;
        ++tmp_counts[val];
    }

    /* sum over all x: add the matching counts for R(x) = x³ – x² */
    for (i = 0; i < m; ++i)
    {
        uint64_t x = i;
        uint64_t rhs = (x * x % m) * ((x + m - 1) % m) % m; /* x³ – x² */
        cnt += tmp_counts[rhs];
    }
    return cnt;
}

/* ------------------------------------------------------------ */
/*  public function a(idx_t n)                                 */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset is 1; 0 is handled harmlessly */
    if (n == 1) return 1;

    if (n > 10000) return 0;         /* out‑of‑bounds for this implementation */

    init_spf();

    if (cache[n] != 0) return cache[n];

    unsigned int nn = (unsigned int)n;
    seq_t result = 1;
    while (nn > 1)
    {
        unsigned int p = spf[nn];
        unsigned int e = 0;
        unsigned int power = 1;
        while (nn % p == 0)
        {
            nn /= p;
            ++e;
            power *= p;            /* power = p^e */
        }
        if (primepow_cnt[power] == 0)
            primepow_cnt[power] = prime_power_count(power);
        result *= primepow_cnt[power];
    }

    cache[n] = result;
    return result;
}
