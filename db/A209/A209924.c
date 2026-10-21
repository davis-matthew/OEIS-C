/*
 * A209924  Concatenation of the months' names spelled in English, with 1=A,
 *          2=B,..., 26=Z.
 * https://oeis.org/A209924
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, word, less
 * Formula:   a(n) = value of the nth letter in the infinite concatenation of
 *            month names (January‑December), mapping A=1..Z
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    static const char *month_names[12] = {
        "January","February","March","April","May","June",
        "July","August","September","October","November","December"
    };
    static const int month_lengths[12] = {7,8,5,5,3,4,4,6,9,7,8,8};
    const idx_t year_len = 74ull;
    idx_t pos = (n-1ull) % year_len;
    int month = 0;
    while (month < 12 && pos >= month_lengths[month]) {
        pos -= month_lengths[month];
        ++month;
    }
    unsigned char c = (unsigned char)month_names[month][pos];
    if (c >= 'a' && c <= 'z') c -= ('a' - 'A');
    return (seq_t)(c - 'A' + 1);
}
