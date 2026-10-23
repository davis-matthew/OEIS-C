/*
 * A011752  Repeat n m times, where m is number of letters in English word for
 *          the number.
 * https://oeis.org/A011752
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, word
 * Formula:   not stated
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) is the smallest integer k such that the sum of the
   letter‑counts of the English words for 0,1,…,k exceeds n.
   (Letter‑count of a number is the number of letters in its conventional
   English name, ignoring spaces and hyphens.) */

static const seq_t SMALL[20] = {
    4, /* zero   */
    3, /* one    */
    3, /* two    */
    5, /* three  */
    4, /* four   */
    4, /* five   */
    3, /* six    */
    5, /* seven  */
    5, /* eight  */
    4, /* nine   */
    3, /* ten    */
    6, /* eleven */
    6, /* twelve */
    8, /* thirteen */
    8, /* fourteen */
    7, /* fifteen */
    7, /* sixteen */
    9, /* seventeen */
    8, /* eighteen */
    8  /* nineteen */
};

static const seq_t TENS[10] = {
    0, /* placeholder */
    0, /* placeholder */
    6, /* twenty  */
    6, /* thirty  */
    5, /* forty   */
    5, /* fifty   */
    5, /* sixty   */
    7, /* seventy */
    6, /* eighty  */
    6  /* ninety  */
};

static const seq_t GROUP_LEN[10] = {
    0,  /* units */
    8,  /* thousand */
    7,  /* million  */
    7,  /* billion  */
    8,  /* trillion */
    10, /* quadrillion */
    10, /* quintillion */
    9,  /* sextillion */
    9,  /* septillion */
    8   /* octillion */
};

/* Length of a number < 1000. */
static seq_t small_len(idx_t n)
{
    if (n < 20)
        return SMALL[n];
    if (n < 100) {
        idx_t t = n / 10;
        idx_t u = n % 10;
        seq_t len = TENS[t];
        if (u)
            len += SMALL[u];
        return len;
    }
    /* n < 1000 */
    idx_t h = n / 100;
    seq_t len = SMALL[h] + 7; /* "hundred" */
    idx_t rem = n % 100;
    if (rem)
        len += small_len(rem);
    return len;
}

/* Length of any non‑negative integer in English words. */
static seq_t word_len_idx(idx_t n)
{
    if (n < 1000)
        return small_len(n);

    seq_t total = 0;
    int group = 0; /* 0 = units, 1 = thousand, ... */
    idx_t temp = n;

    while (temp > 0) {
        idx_t part = temp % 1000;
        if (part) {
            total += small_len(part);
            if (group > 0)
                total += GROUP_LEN[group];
        }
        temp /= 1000;
        ++group;
    }
    return total;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    idx_t target = n; /* Keep comparison in idx_t to avoid truncation. */
    seq_t cumulative = 0;
    idx_t k = 0;
    for (;; ++k) {
        cumulative += word_len_idx(k);
        if ((idx_t)cumulative > target)
            return (seq_t)k;
    }
}
