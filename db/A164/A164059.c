/*
 * A164059  Number of straight plus curved segments in the capitalized English
 *          name of n.
 * https://oeis.org/A164059
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, word
 * Formula:   a(n) = sum of segment counts of each letter in the capitalized
 *            English name of n. Segment counts: A=3, B=3, C=1, D=2, E=4, F=3,
 *            G=3, H=3, I=1, J=2, K=3, L=2, M=4, N=3, O=1, P=2, Q=2, R=3, S=1,
 *            T=2, U=1, V=2, W=4, X=2, Y=3, Z=3
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const seq_t letter_counts[26] = {
    3, 3, 1, 2, 4, 3, 3, 3, 1, 2, 3, 2, 4, 3, 1, 2, 2, 3, 1, 2, 1, 2, 4, 2, 3, 3
};

static const char *const units[10] = {
    "", "ONE", "TWO", "THREE", "FOUR", "FIVE",
    "SIX", "SEVEN", "EIGHT", "NINE"
};

static const char *const teens[20] = {
    "TEN", "ELEVEN", "TWELVE", "THIRTEEN", "FOURTEEN",
    "FIFTEEN", "SIXTEEN", "SEVENTEEN", "EIGHTEEN", "NINETEEN"
};

static const char *const tens[10] = {
    "", "", "TWENTY", "THIRTY", "FORTY",
    "FIFTY", "SIXTY", "SEVENTY", "EIGHTY", "NINETY"
};

static seq_t add_word(const char *w, seq_t *sum)
{
    while (*w) {
        char c = *w++;
        if (c == ' ' || c == '-') continue;
        if (c >= 'A' && c <= 'Z')
            *sum += letter_counts[c - 'A'];
    }
    return *sum;
}

static void add_below_1000(idx_t n, seq_t *sum)
{
    if (n >= 100) {
        add_below_1000(n / 100, sum);
        add_word("HUNDRED", sum);
        idx_t rem = n % 100;
        if (rem) {
            if (rem < 20) {
                if (rem < 10) add_word(units[rem], sum);
                else          add_word(teens[rem - 10], sum);
            } else {
                add_word(tens[rem / 10], sum);
                if (rem % 10) add_word(units[rem % 10], sum);
            }
        }
    } else if (n >= 20) {
        add_word(tens[n / 10], sum);
        if (n % 10) add_word(units[n % 10], sum);
    } else if (n >= 10) {
        add_word(teens[n - 10], sum);
    } else if (n > 0) {
        add_word(units[n], sum);
    }
}

static void add_number(idx_t n, seq_t *sum)
{
    if (n >= 1000000000ULL) {
        add_number(n / 1000000000ULL, sum);
        add_word("BILLION", sum);
        idx_t rem = n % 1000000000ULL;
        if (rem) add_number(rem, sum);
    } else if (n >= 1000000ULL) {
        add_number(n / 1000000ULL, sum);
        add_word("MILLION", sum);
        idx_t rem = n % 1000000ULL;
        if (rem) add_number(rem, sum);
    } else if (n >= 1000ULL) {
        add_number(n / 1000ULL, sum);
        add_word("THOUSAND", sum);
        idx_t rem = n % 1000ULL;
        if (rem) add_number(rem, sum);
    } else {
        add_below_1000(n, sum);
    }
}

seq_t a(idx_t n)
{
    seq_t total = 0;
    if (n == 0)
        add_word("ZERO", &total);
    else
        add_number(n, &total);
    return total;
}
