/*
 * A001050  Number of letters in n (in Finnish).
 * https://oeis.org/A001050
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, word
 * Formula:   a(n) = number of letters in the Finnish name of n, built from
 *            units nolla..yhdeksän, teens <unit>toista, tens <unit>kymmentä,
 *            sata/<unit>sataa, tuhat/<n>tuhatta, miljoona/<n>miljoonaa,
 *            miljardi/<n>miljardia (ä counts as one letter)
 * Range:     u8 n <= 9889, u16 n <= 9889, u32 n <= 9889, u64 n <= 9889,
 *            u128 n <= 9889, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const int fi_units[10] = {5, 4, 5, 5, 5, 5, 5, 9, 9, 8};

static int fi_len(idx_t n) {
    if (n < 10) return fi_units[n];
    if (n < 100) {
        idx_t t = n / 10, u = n % 10;
        if (t == 1) {
            if (u == 0) return 8;                  /* kymmenen */
            return fi_units[u] + 6;                /* -toista */
        }
        return fi_units[t] + 8 + (u ? fi_units[u] : 0); /* -kymmentä */
    }
    if (n < 1000) {
        idx_t h = n / 100, r = n % 100;
        int res = (h == 1) ? 4 : fi_units[h] + 5;  /* sata / -sataa */
        return res + (r ? fi_len(r) : 0);
    }
    if (n < 1000000ULL) {
        idx_t k = n / 1000, r = n % 1000;
        int res = (k == 1) ? 5 : fi_len(k) + 7;    /* tuhat / -tuhatta */
        return res + (r ? fi_len(r) : 0);
    }
    if (n < 1000000000ULL) {
        idx_t m = n / 1000000ULL, r = n % 1000000ULL;
        int res = (m == 1) ? 8 : fi_len(m) + 9;    /* miljoona / -miljoonaa */
        return res + (r ? fi_len(r) : 0);
    }
    {
        idx_t b = n / 1000000000ULL, r = n % 1000000000ULL;
        int res = (b == 1) ? 8 : fi_len(b) + 9;    /* miljardi / -miljardia */
        return res + (r ? fi_len(r) : 0);
    }
}

seq_t a(idx_t n) {
    seq_t r = 0;
    r += (seq_t)fi_len(n);
    return r;
}
